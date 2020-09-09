def getLength(arr):
    c=0
    for x in arr:
        if type(x) == list:
            c += getLength(x)
        else:
            c += 1

    return c


print(getLength([1,[2,[3,[4,[5, 6]]]]]))
