// ResourceBase.cpp : implementation file
//

#include "stdafx.h"
#include "ResourceBase.h"

IResourceBase *IResourceBase::Clone() const
{
    IResourceBase *pResource = NULL;
    sci::istream serial;
    if (Serialize(&serial))
    {
        if (serial.GetDataSize() <= MaxResourceSize)
        {
            if (serial.Seek(0))
            {
                // Note: we're allocating a second copy of the bits here.  If that becomes an issue, we
                // could make a ResourceBlob constructor that "borrows" the bits.
                ResourceBlob data;
                if (SUCCEEDED(data.CreateFromBits(NULL, GetType(), &serial, GetPackageNumber(), GetResourceNumber())))
                {
                    CreateResourceFromResourceData(&data, &pResource);
                }
            }
        }
        else
        {
            // If we serialized more than MaxResourceSize bytes,
            // then 1) SCI can't handle it.  2) The internal structure of the data
            // could very well be corrupt (could depend on (WORD) casts).  So fail.
        }
    }
    return pResource;
}


SCI_RESOURCE_INFO g_resourceInfo[10] =
{
    {  TEXT("view"), TEXT("View"), TEXT("View resources (view.*)|view.*|All files (*.*)|*.*|") },
    {  TEXT("pic"), TEXT("Pic"), TEXT("Pic resources (pic.*)|pic.*|All files (*.*)|*.*|") },
    {  TEXT("script"), TEXT("Script"), TEXT("Script resources (script.*)|script.*|All files (*.*)|*.*|") },
    {  TEXT("text"), TEXT("Text"), TEXT("Text resources (text.*)|text.*|All files (*.*)|*.*|") },
    {  TEXT("sound"), TEXT("Sound"), TEXT("Sound resources (sound.*)|sound.*|All files (*.*)|*.*|") },
    {  TEXT("memory"), TEXT("Memory"), TEXT("Memory resources (memory.*)|memory.*|All files (*.*)|*.*|") },
    {  TEXT("vocab"), TEXT("Vocab"), TEXT("Vocab resources (vocab.*)|vocab.*|All files (*.*)|*.*|") },
    {  TEXT("font"), TEXT("Font"), TEXT("Font resources (font.*)|font.*|All files (*.*)|*.*|") },
    {  TEXT("cursor"), TEXT("Cursor"), TEXT("Cursor resources (cursor.*)|cursor.*|All files (*.*)|*.*|") },
    {  TEXT("patch"), TEXT("Patch"), TEXT("Patch resources (patch.*)|patch.*|All files (*.*)|*.*|") },
};

DWORD g_celResourceInfo = ARRAYSIZE(g_resourceInfo);

