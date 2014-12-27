import numpy as np
import cv2

cap = cv2.VideoCapture(0)
w=int(cap.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH ))
h=int(cap.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT ))
out = cv2.VideoWriter('output.avi', -1, 20.0, (w,h))

while(cap.isOpened()):
	# capture frame-by-frame
	ret, frame = cap.read()
	if(ret == True):
		frame = cv2.flip(frame, 0)

		# write file
		out.write(frame)

		# display
		cv2.imshow("Video", frame)
		if(cv2.waitKey(1) & 0xFF == ord('q')):
			break

# release
cap.release()
out.release()
cv2.destroyAllWindows()