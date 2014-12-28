import numpy as np
import cv2

def mouse_cb(event, x, y, flags, param):
	global ix, iy, drawing, mode

	if event == cv2.EVENT_LBUTTONDOWN:
		drawing = True
		ix, iy = x, y

	elif event == cv2.EVENT_MOUSEMOVE:
		if drawing == True:
			cv2.rectangle(img, (ix, iy), (x,y), (0,255,0), -1)

	elif event == cv2.EVENT_LBUTTONUP:
		drawing = False
		cv2.rectangle(img, (ix, iy), (x,y), (0,255,0), -1)

img = np.zeros((512, 512, 3), np.uint8)
drawing = False
cv2.namedWindow("Mouse")
cv2.setMouseCallback("Mouse", mouse_cb)

while(1):
	cv2.imshow("Mouse", img)
	k = cv2.waitKey(1) & 0xFF
	if k == 27:
		break

cv2.destroyAllWindows()
