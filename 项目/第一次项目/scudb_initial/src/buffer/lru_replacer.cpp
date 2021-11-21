/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

template <typename T> LRUReplacer<T>::LRUReplacer() {
    head = make_shared<Node>();
    head->next = tail;
    tail = make_shared<Node>();
    tail->prev = head;
}

template <typename T> LRUReplacer<T>::~LRUReplacer() {}

/*
 * Insert value into LRU
 */
template <typename T> void LRUReplacer<T>::Insert(const T &value) {
    lock_guard<mutex> lock(latch);
    shared_ptr<Node> cur;
    if (map.find(value) != map.end())// if the value has been in the list, refresh this node to the first
    {
        cur = map[value];
        shared_ptr <Node> prev = cur->prev;
        shared_ptr <Node> succ = cur->next;
        prev->next = succ;
        succ->prev = prev;
    }
    else // else insert a new node to the first
    {
        cur = make_shared<Node>(value);
    }
    shared_ptr<Node> fir = head->next;
    cur->next = fir;
    fir->prev = cur;
    cur->prev = head;
    head->next = cur;
    map[value] = cur;  // refresh the map
    return;// }// }
}

/* If LRU is non-empty, pop the head member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
template <typename T> bool LRUReplacer<T>::Victim(T &value) {
    lock_guard<mutex> lock(latch);
    if (map.empty()) {
        return false;
    }
    shared_ptr<Node> last = tail->prev;
    tail->prev = last->prev;
    last->prev->next = tail;
    value = last->val;
    map.erase(last->val);
    return true;
}

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
    lock_guard<mutex> lock(latch);
    if (map.find(value) != map.end())
    {
        shared_ptr <Node> cur = map[value];
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
    }
    return map.erase(value);
}

template <typename T> size_t LRUReplacer<T>::Size() {
    lock_guard<mutex> lock(latch);
    return map.size();
}

template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace scudb
