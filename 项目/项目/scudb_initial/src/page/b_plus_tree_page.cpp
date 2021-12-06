/**
 * b_plus_tree_page.cpp
 */
#include "page/b_plus_tree_page.h"

namespace scudb {

/*
 * Helper methods to get/set page type
 * Page type enum class is defined in b_plus_tree_page.h
 */
bool BPlusTreePage::IsLeafPage() const 
{
  return page_type_ == IndexPageType::LEAF_PAGE;
}

bool BPlusTreePage::IsRootPage() const 
{
  return parent_page_id_ == INVALID_PAGE_ID;
}

void BPlusTreePage::SetPageType(IndexPageType pt) 
{
  page_type_ = pt;
}

/*
 * Helper methods to get/set size (number of key/value pairs stored in that
 * page)
 */
int BPlusTreePage::GetSize() const
{
  return size_;
}

void BPlusTreePage::SetSize(int si)
{
  size_ = si;
}

void BPlusTreePage::IncreaseSize(int num)
{
  size_ += num;
}

/*
 * Helper methods to get/set max size (capacity) of the page
 */
int BPlusTreePage::GetMaxSize() const 
{
  return max_size_;
}

void BPlusTreePage::SetMaxSize(int si)
{
  max_size_ = si;
}

/*
 * Helper method to get min page size
 * Generally, min page size == max page size / 2
 * With n = 4 in our example B+-tree, each leaf must contain at least 2 values, and at most 3 values.
 */
int BPlusTreePage::GetMinSize() const
{
  if (IsRootPage())
  {
    return IsLeafPage() ? 1:2;
  }
  return (max_size_ )/2;
}

/*
 * Helper methods to get/set parent page id
 */
page_id_t BPlusTreePage::GetParentPageId() const
{
  return parent_page_id_;
}

void BPlusTreePage::SetParentPageId(page_id_t parent_page_id) 
{ 
  parent_page_id_ = parent_page_id;
}

/*
 * Helper methods to get/set self page id
 */
page_id_t BPlusTreePage::GetPageId() const 
{ 
  return page_id_; 
}

void BPlusTreePage::SetPageId(page_id_t page_id) 
{ 
  page_id_ = page_id; 
}

/*
 * Helper methods to set lsn
 */
void BPlusTreePage::SetLSN(lsn_t lsn) 
{ 
  lsn_ = lsn; 
}

/* for concurrent index */
bool BPlusTreePage::IsSafe(OpType op) 
{
  int size = GetSize();
  if (op == OpType::INSERT) 
  {
    return size < GetMaxSize();
  }
  int minSize = GetMinSize() + 1;
  if (op == OpType::DELETE) 
  {
    return (IsLeafPage()) ? size >= minSize : size > minSize;
  }
  assert(false);
}
} // namespace scudb
