//////////////////////////////////////////////////////////////////////////
//IniFile.h
//ini文件读写类
//楚茗版权所有
//2005年7月6日
//你可以免费使用、修改和传播，但请保留作者版权信息
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_INIFILE_H__B5C0D7F7_8353_4C93_AAA4_38A688CA253C__INCLUDED_)
#define AFX_INIFILE_H__B5C0D7F7_8353_4C93_AAA4_38A688CA253C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile
{
public:
    CIniFile();
    virtual ~CIniFile();

    BOOL SetPath(CString strPath);//设置ini文件路径,成功TRUE;否则FALSE

    BOOL Createini();//创建m_strPath中的ini文件

    BOOL SectionExist(CString strSection);//检查section是否存在,存在TRUE;否则FALSE,该section下无key也做不存在处理并返回FALSE

    CString GetKeyValue(CString strSection,CString strKey);//从指定的Section和Key读取KeyValue

    void SetKeyValue(CString strSection,CString strKey,CString strKeyValue);//设置/修改Section、Key以及KeyValue，若Section或者Key不存在则创建

    void DeleteKey(CString strSection,CString strKey);//删除指定Section下的一个Key

    void DeleteSection(CString strSection);//删除指定的Section以及其下的所有Key

    int GetAllSections(CStringArray& strArrSection);//获得所有的Section,返回Section数目


    int GetAllKeysAndValues (CString strSection,CStringArray&strArrKey,CStringArray&strArrKeyValue);//根据指定Section得到其下的所有Key和KeyValue,返回Key的数目

    void DeleteAllSections();//删除所有Section

private:
    //ini文件路径
    CString m_strPath;
};

#endif // !defined(AFX_INIFILE_H__B5C0D7F7_8353_4C93_AAA4_38A688CA253C__INCLUDED_)
//////////////////////////////////////////////////////////////////////////
//实现CString类型到int类型的转换,例如CString str1="5";int i=atoi(str1);
//实现CString类型到double类型的转换,例如CString str2="5";double j=atoi(str2);
//////////////////////////////////////////////////////////////////////////