# 不知为何，不做线程同步结果也没什么问题
import time
import threading
# from queue import Queue  # 原生支持锁原语

sum = 0
lock = threading.Lock()

def add_t():
    print("add 线程启动")
    global sum
    for i in range(100):
        lock.acquire()
        try:
            sum += 1
        finally:
            lock.release()
        # print("{}: add".format(i))
    print("add 线程结束")

def sub_t():
    print("sub 线程启动")
    global sum
    for i in range(100):
        lock.acquire()
        try:
            sum -= 1
        finally:
            lock.release()
        # print("{}: sub".format(i))
    print("sub 线程结束")


if __name__ == '__main__':
    add_thread = threading.Thread(target=add_t)
    sub_thread = threading.Thread(target=sub_t)

    add_thread.start()
    sub_thread.start()

    add_thread.join()
    sub_thread.join()

    print("result: {}".format(sum))

    # print(time.ctime(time.time()))
