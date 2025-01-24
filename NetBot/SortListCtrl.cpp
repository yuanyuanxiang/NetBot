#include "StdAfx.h"
#include "SortListCtrl.h"

#include <stdlib.h>
#include <math.h>

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
    int nItem1,nItem2;

    LVFINDINFO FindInfo;
    FindInfo.flags = LVFI_PARAM;
    FindInfo.lParam = lParam1;
    nItem1 = pList->FindItem(&FindInfo, -1);
    FindInfo.lParam = lParam2;
    nItem2 = pList->FindItem(&FindInfo, -1);

    if((nItem1 == -1) || (nItem2 == -1)) {
        return 0;
    }

    CString str1, str2;
    str1 = pList->GetItemText(nItem1, pList->m_nSortedCol);
    str2 = pList->GetItemText(nItem2, pList->m_nSortedCol);

    HDITEM headerItem;
    headerItem.mask = HDI_LPARAM;
    CHeaderCtrl* pHeaderCtrl = pList->GetHeaderCtrl( );
    pHeaderCtrl->GetItem(pList->m_nSortedCol, &headerItem);
    UINT nType = (UINT)(headerItem.lParam);

    int iCompRes = 0;
    switch(nType) {
    case CSortListCtrl::DATA_TYPE::INT_TYPE: {
        int i1,i2;
        char str[256] = {0};

        strcpy(str,str1);

        i1 = atoi(str);
        strcpy(str,str2);

        i2 = atoi(str);

        if( i1 > i2)
            iCompRes = 1;
        else if( i1 == i2 )
            iCompRes = 0;
        else
            iCompRes = -1;
    }
    break;

    case CSortListCtrl::DATA_TYPE::STRING_TYPE:		//throughout
    default: {
        char strf[256] = {0},strn[256] = {0};
        strcpy(strf,str1);
        strcpy(strn,str2);

        iCompRes = strcmp(strf,strn);
    }
    break;
    }

    if(pList->m_bAsc)
        return iCompRes;
    else
        return iCompRes * -1;

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

    //非常重要，不能遗漏
    //********************************************************
    int num = this->GetItemCount();
    while(num--)
        this->SetItemData(num, num);
    //********************************************************

    SortItems(ListCompare, (DWORD)this);

    UpdateData(FALSE);

    *pResult = 0;
}
