#pragma once


template<typename _InIt>
_InIt match_name(_InIt _First, _InIt _Last, const std::string &name)
{
    for (;_First != _Last; _First++)
    {
        if ((*_First)->GetName() == name)
        {
            break;
        }
    }
    return _First;
}

template<typename _InIt>
_InIt match_name_dot(_InIt _First, _InIt _Last, const std::string &name)
{
    for (;_First != _Last; _First++)
    {
        if ((*_First).GetName() == name)
        {
            break;
        }
    }
    return _First;
}


template<typename _InIt>
bool matches_name(_InIt _First, _InIt _Last, const std::string &name)
{
    _First = match_name(_First, _Last, name);
    return _First != _Last;
}


// Used for looking up items in const array where a string maps something to another value.
template<typename _TKey, typename _TValue>
struct key_value_pair
{
    _TKey key;
    _TValue value;
};

template<typename _T>
_T LookupStringValue(const key_value_pair<PCSTR, _T> *rgArray, size_t size, const std::string &name, _T defaultValue = NULL)
{
    _T value = defaultValue;
    bool fRet = FALSE;
    for (size_t i = 0; !fRet && i < size; i++)
    {
        fRet = (name == rgArray[i].key);
        if (fRet)
        {
            value = rgArray[i].value;
        }
    }
    return value;
}

template<typename _TKey, typename _TValue>
_TValue LookupKeyValue(const key_value_pair<_TKey, _TValue> *rgArray, size_t size, const _TKey &key, const _TValue &defValue)
{
    _TValue value = defValue;
    for (size_t i = 0; i < size; i++)
    {
        if (key == rgArray[i].key)
        {
            value = rgArray[i].value;
            break;
        }
    }
    return value;
}

template<typename _TKey, typename _TValue>
_TKey LookupKey(const key_value_pair<_TKey, _TValue> *rgArray, size_t size, const _TValue &value, const _TKey &defKey)
{
    _TKey key = defKey;
    for (size_t i = 0; i < size; i++)
    {
        if (value == rgArray[i].value)
        {
            key = rgArray[i].key;
            break;
        }
    }
    return key;
}

template<typename _TKey, typename _TValue>
bool FindKeyValue(const key_value_pair<_TKey, _TValue> *rgArray, size_t size, const _TKey &key, _TValue &value)
{
    bool fRet = false;
    for (size_t i = 0; !fRet && i < size; i++)
    {
        fRet = (key == rgArray[i].key);
        if (fRet)
        {
            value = rgArray[i].value;
        }
    }
    return fRet;
}

template<typename _TKey, typename _TValue>
bool FindKey(const key_value_pair<_TKey, _TValue> *rgArray, size_t size, const _TValue &value, _TKey &key)
{
    bool fRet = false;
    for (size_t i = 0; !fRet && i < size; i++)
    {
        fRet = (value == rgArray[i].value);
        if (fRet)
        {
            key = rgArray[i].key;
        }
    }
    return fRet;
}



void push_word(std::vector<BYTE> &output, WORD w);
void write_word(std::vector<BYTE> &output, size_t index, WORD w);
void push_string(std::vector<BYTE> &output, const std::string &str);
void push_string_rle(std::vector<BYTE> &output, const std::string &str);
void push_string_nt(std::vector<BYTE> &output, const std::string &str);
WORD push_temp(std::vector<BYTE> &output);

template<typename _TContainer, typename _InIt>
void push_strings_nt(_TContainer &cont, _InIt _First, _InIt _Last)
{
    for (;_First != _Last; ++_First)
    {
        push_string_nt(cont, *_First);
    }
}