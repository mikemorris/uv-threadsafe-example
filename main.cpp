#include <memory>
#include <map>
#include <iostream>

// libuv
#include <uv.h>

template <class K, class V, class Compare = std::less<K>,
          class Allocator = std::allocator<std::pair<const K, V> > >

class guarded_map {
  private:
    typedef std::map<K, V, Compare, Allocator> map_type;
    typedef std::lock_guard<std::mutex> lock_guard;

    map_type map;
    std::mutex mutex;

  public:
    typedef typename map_type::const_iterator const_iterator;

    bool empty() {
      lock_guard guard(this->mutex);
      return this->map.empty();
    }

    const_iterator cbegin() {
      lock_guard guard(this->mutex);
      return this->map.cbegin();
    }

    const_iterator cend() {
      lock_guard guard(this->mutex);
      return this->map.cend();
    }

    std::pair<const_iterator, bool> emplace(K key, V value) {
      lock_guard guard(this->mutex);
      return this->map.emplace(key, value);
    }

    const_iterator find(K key) {
      lock_guard guard(this->mutex);
      return this->map.find(key);
    }

    std::pair<const_iterator, bool> insert(K key, V value) {
      lock_guard guard(this->mutex);
      return this->map.emplace(key, value);
    }

    size_t size() {
      lock_guard guard(this->mutex);
      return this->map.size();
    }
};

typedef guarded_map<int, bool> map_type;

struct Baton {
  int num;
  std::shared_ptr<map_type> map;
};

void AsyncReadWritemap(uv_work_t* req) {
  Baton* baton = static_cast<Baton*>(req->data);
  std::shared_ptr<map_type> const map = baton->map;
  int num = baton->num;

  map->emplace(num, true);
  map_type::const_iterator itr = map->find(num);

  std::cout << itr->first << ' ';
}

int main() {
  auto map_ptr = std::make_shared<map_type>();

  for (int i = 0; i < 10000; i++) {
    Baton* baton = new Baton();
    baton->num = i;
    baton->map = map_ptr;

    uv_work_t *req = new uv_work_t();
    req->data = baton;

    uv_queue_work(uv_default_loop(), req, AsyncReadWritemap, (uv_after_work_cb)nullptr);
  }
}
