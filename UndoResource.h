#pragma once

//
// A generic undo for sci resources.
// _TBase is the baseclass of the MFC document object to which you want to add this functionality
// _TItem is the resource type
//

#define MAX_UNDO 100
#define AGGRESSIVITY_UNDO 30

template <class _TBase, class _TItem>
class CUndoResource : public _TBase
{
public:
    typedef std::list<_TItem*> _MyListType;

    CUndoResource()
    {
        _pos = _undo.end();
    }

    virtual ~CUndoResource()
    {
        // Delete our undo buffer
        for_each(_undo.begin(), _undo.end(), delete_object());
    }

    void AddFirstResource(_TItem *pResource)
    {
        ASSERT(_pos == _undo.end());
        _pLastSaved = pResource;
        _undo.push_back(pResource);
        _pos = _GetLastUndoFrame();
    }

    void AddNewResourceToUndo(_TItem *pResourceNew)
    {
        // Delete all resources after the current one.
        std::list<_TItem*>::iterator pos = _pos;
        ++pos;
        // Now pos points to the position after the current one.
        while (pos != _undo.end())
        {
            // Delete all those resources.
            delete *pos;
            std::list<_TItem*>::iterator posToDel = pos;
            ++pos;
            _undo.erase(posToDel);
        }

        // Insert after the current pos (which is now the end), and make this our new pos.
        _undo.push_back(pResourceNew);
        _pos = _GetLastUndoFrame();

        // Make sure we don't grow infinitely.
        _TrimUndoStack();
    }

    void SetLastSaved(const _TItem *pResource)
    {
        _pLastSaved = pResource;
    }

    // Gets the resource at the current location
    const _TItem *GetResource() const
    {
        return (_pos != _undo.end()) ? *_pos : NULL;
    }

    bool HasUndos()
    {
        return !_undo.empty();
    }

protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnUndo()
    {
        if (_pos != _undo.begin())
        {
            --_pos;
            _OnUndoRedo();
            SetModifiedFlag(*_pos != _pLastSaved);
        }
    }

    afx_msg void OnRedo()
    {
        if (_pos != _GetLastUndoFrame())
        {
            ++_pos;
            _OnUndoRedo();
            SetModifiedFlag(*_pos != _pLastSaved);
        }
    }

    afx_msg void OnUpdateRedo(CCmdUI *pCmdUI)
    {
        BOOL fRet = FALSE;
        if (!_undo.empty() && !_PreventUndos())
        {
            fRet = (_pos != _GetLastUndoFrame());
        }
        pCmdUI->Enable(fRet);
    }

    afx_msg void OnUpdateUndo(CCmdUI *pCmdUI)
    {
        BOOL fRet = FALSE;
        if (!_undo.empty() && !_PreventUndos())
        {
            fRet = (_pos != _undo.begin());
        }
        pCmdUI->Enable(fRet);
    }

    // Overrides
    virtual bool v_PreventUndos() const
    {
        return false;
    }

    virtual void v_OnUndoRedo() = 0;

private:
    bool _PreventUndos()
    {
        return v_PreventUndos();
    }

    void _OnUndoRedo()
    {
        v_OnUndoRedo();
    }

    void _TrimUndoStack()
    {
        if (_undo.size() > MAX_UNDO)
        {
            // Remove every other stored frame, up to a certain point.
            std::list<_TItem*>::iterator pos = _undo.begin();
            BOOL fRemove = FALSE;
            for (int i = 0; i < AGGRESSIVITY_UNDO; i++)
            {
                if (pos != _pos) // Skip it if it's where we currently are!
                {
                    std::list<_TItem*>::iterator posToMaybeDelete = pos;
                    _TItem *pResource = *pos;
                    ++pos;
                    if (fRemove)
                    {
                        delete pResource;
                        _undo.erase(posToMaybeDelete);
                    }
                    fRemove = !fRemove;
                }
            }
        }
    }

    typename std::list<_TItem*>::iterator _GetLastUndoFrame()
    {
        std::list<_TItem*>::iterator pos = _undo.end();
        return --pos;
    }

    // Undo buffer.
    _MyListType _undo;
    typename std::list<_TItem*>::iterator _pos;

    const _TItem *_pLastSaved; // Weak ref
};


BEGIN_TEMPLATE_MESSAGE_MAP( TEMPLATE_2(class _TBase, class _TItem) , TCLASS_2(CUndoResource, _TBase, _TItem), _TBase)
    ON_COMMAND(ID_EDIT_UNDO, OnUndo)
    ON_COMMAND(ID_EDIT_REDO, OnRedo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateRedo)
END_MESSAGE_MAP()

