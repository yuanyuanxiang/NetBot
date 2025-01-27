#include "StdAfx.h"
#include "SortListCtrl.h"

#include <stdlib.h>
#include <math.h>
#include "../Seu_lib/Command.h"

CSortListCtrl::CSortListCtrl(void)
    : m_bAsc(false)
    , m_nSortedCol(0)
{

}

CSortListCtrl::~CSortListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, CSortListCtrl::OnLvnColumnclick)
END_MESSAGE_MAP()

int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CSortListCtrl* pList = (CSortListCtrl*)lParamSort;
    LPFileInfo f1 = (LPFileInfo)lParam1;
    LPFileInfo f2 = (LPFileInfo)lParam2;
    int asc = pList->m_bAsc ? 1 : -1;
    switch (pList->m_nSortedCol)
    {
    case 0:// 文件名
        return strcmp(f1->cFileName, f2->cFileName) < 0 ? asc : -asc;
    case 1:// 类型
        return f1->iType < f2->iType ?  asc : -asc;
    case 2:// 大小
        return strcmp(f1->cSize, f2->cSize) < 0 ? asc : -asc;
    case 3:// 修改时间
        return strcmp(f1->cTime, f2->cTime) < 0 ? asc : -asc;
    default:
        break;
    }
    return 0;
}

void CSortListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    // TODO: 在此添加控件通知处理程序代码
    if(pNMLV->iSubItem == m_nSortedCol) {
        m_bAsc = !m_bAsc;
    } else {
        m_bAsc = TRUE;
        m_nSortedCol = pNMLV->iSubItem;
    }

    SortItems(ListCompare, (DWORD)this);

    UpdateData(FALSE);

    *pResult = 0;
}
