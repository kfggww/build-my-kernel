// nolocal goto到已经返回的函数会发生什么事情呢? 在这个例子中, 在调用
// test_func的时候, 第一次setjmp返回0, 函数退出; 之后执行到longjmp, 再
// 次转到test_func的setjmp调用处, 注意, 此时使用的test_func的栈帧理论
// 上已经不存在, 但实际上内存中还有, 这次setjmp返回的是1, 结果
// test_func再次返回.
#include "../apue.h"

static jmp_buf env;

void test_func() {

  switch(setjmp(env)) {
  case 0:
    printf("test func return by 0\n");
    return;
  case 1:
    printf("test func return by 1\n");
    return;
  }
  
}

int main(int argc, char **argv) {

  test_func();
  longjmp(env, 1);
  return 0;
}
