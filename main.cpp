#include <atomic>
#include <memory>
#include <vector>
#include <iostream>

// libuv
#include <uv.h>

typedef std::vector<int> vector_type;

struct Baton {
  int num;
  std::shared_ptr<vector_type> ptr;
};

void AsyncReadWriteVector(uv_work_t* req) {
  Baton* baton = static_cast<Baton*>(req->data);
  std::shared_ptr<vector_type> ptr = baton->ptr;

  auto vector = std::atomic_load(&ptr);
  vector->push_back(baton->num);

  for (auto const& num : *vector) {
    std::cout << num << ' ';
  }

  std::cout<< '\n';
}

int main() {
  std::shared_ptr<vector_type> vector_ptr(new vector_type);

  for (int i = 0; i < 100; i++) {
    Baton* baton = new Baton();
    baton->num = i;
    baton->ptr = vector_ptr;

    uv_work_t *req = new uv_work_t();
    req->data = baton;

    uv_queue_work(uv_default_loop(), req, AsyncReadWriteVector, (uv_after_work_cb)nullptr);
  }
}
