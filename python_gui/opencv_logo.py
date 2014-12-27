import numpy as np
import cv2

# blank canvas
image = np.zeros((512, 512, 3), np.uint8) + 255

# draw circles
cv2.ellipse(image, (256, 120), (60, 60), 120, 0, 300, (0, 0, 255), 35, cv2.CV_AA)
cv2.ellipse(image, (170, 280), (60, 60), 0, 0, 300, (0, 255, 0), 35, cv2.CV_AA)
cv2.ellipse(image, (342, 280), (60, 60), -60, 0, 300, (255, 0, 0), 35, cv2.CV_AA)

# draw font
font = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText(image, 'OpenCV', (75, 450), font, 3, 0, 7, cv2.CV_AA)

cv2.imshow("OpenCV Logo", image)
cv2.waitKey(0)
cv2.destroyAllWindows()