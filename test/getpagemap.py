for i in range(9999999999):
    text = f.read(8)
    if text != '\x00\x00\x00\x00\x00\x00\x00\x00':
        print(text)
    
    if i % 10000000 == 0:
        print(f.tell())