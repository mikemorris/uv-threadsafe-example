#include <atomic>
#include <memory>
#include <map>
#include <iostream>

// libuv
#include <uv.h>

template <class K, class V, class Compare = std::less<K>,
          class Allocator = std::allocator<std::pair<const K, V> > >

class guarded_map {
  private:
    std::map<K, V, Compare, Allocator> map;
    std::mutex mutex;

  public:
    void emplace(K key, V value) {
      std::lock_guard<std::mutex> lock(this->mutex);
      this->map.emplace(key, value);
    }

    V & get(K key) {
      std::lock_guard<std::mutex> lock(this->mutex);
      return this->map[key];
    }

    bool empty() {
      std::lock_guard<std::mutex> lock(this->mutex);
      return this->map.empty();
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

  std::cout << map->get(num) << ' ';
}

int main() {
  auto map_ptr = std::make_shared<map_type>();

  for (int i = 0; i < 1000; i++) {
    Baton* baton = new Baton();
    baton->num = i;
    baton->map = map_ptr;

    uv_work_t *req = new uv_work_t();
    req->data = baton;

    uv_queue_work(uv_default_loop(), req, AsyncReadWritemap, (uv_after_work_cb)nullptr);
  }
}
