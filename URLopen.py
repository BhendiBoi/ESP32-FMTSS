import urllib.request

if mask_detected:
  urllib.request.urlopen('http://192.168.43.136/mask')
else:
  urllib.request.urlopen('http://192.168.43.136/nomask')
