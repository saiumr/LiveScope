print("#### 转set取差集")
a = [1,0,4,1,3]
b = [1,5,3,4,2]
print(list(set(a) - set(b)))  # a独有
print(list(set(b) - set(a)))  # b独有
print(list(set(a) ^ set(b)))  # a b 独有的并集

print("#### 列表推导式")
a = [1,0,4,1,3]
b = [1,5,3,4,2]
diff_a = [x for x in a if x not in b]
diff_b = [x for x in b if x not in a]
print(diff_a)
print(diff_b)


print("#### 转set再列表推导式取差集")
a = [1,0,4,1,3]
b = [1,5,3,4,2]
set_a = set(a)
set_b = set(b)
diff_a = [x for x in a if x not in set_b]
diff_b = [x for x in b if x not in set_a]
print(diff_a)
print(diff_b)
