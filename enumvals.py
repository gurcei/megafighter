f = open("bitmap_ids.h")
text = f.read()
rows = text.split(',')
cnt = 0
for row in rows:
  print(row + ' = ' + str(cnt))
  cnt += 1
