# REDIS_NAME   代替编译输出文件名
# REDIS_OBJS   代替  依赖文件
# CC     代替  gcc
# CFLAGS 代替  编译命令
# $^ 代替 上面的指令
# RM 代替 rm -f
# $@ 代替 目标文件
#      %.o:%.c
#	       $(CC) $^ $(CFLAGS)  -o $@
# 百分号相当于一个通配符

CC=GCC
CFLAGS=-c -Wall -g

REDIS_NAME = redis-test
REDIS_OBJS = main.o sds.o

redis: $(REDIS_NAME)

$(REDIS_NAME): $(REDIS_OBJ)
	$(CC) $^ -o $@

%.o:%.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	$(RM) *.o $(REDIS_NAME) -r
