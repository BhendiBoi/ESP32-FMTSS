import urllib.request

if mask_detected:
  urllib.request.urlopen('http://192.168.43.136/H')
else:
  urllib.request.urlopen('http://192.168.43.136/L')
