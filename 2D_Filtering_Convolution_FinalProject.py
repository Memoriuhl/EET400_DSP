import cv2
import numpy as np
from matplotlib import pyplot as plt
from skimage import io, color
from skimage import exposure

# Finding image file and converting to RGB instead of BGR
raw_img = cv2.imread('/Users/memoriuhl/PycharmProjects/OpenCVTesting/wildcat.jpg')
img=cv2.cvtColor(raw_img,cv2.COLOR_BGR2RGB)

def cv2_images(img):
    # Blurs the images by various methods
    img = io.imread(r'/Users/memoriuhl/PycharmProjects/OpenCVTesting/wildcat.jpg')
    box_blur = np.array([[-1, -1, -1], [-1, 8, -1], [-1, -1, -1]])
    blur = cv2.blur(img, (5, 5))
    BLUR = cv2.filter2D(img, -1, box_blur)
    median = cv2.medianBlur(img, 5)
    bilat = cv2.bilateralFilter(img, 9, 300, 300)
    gaussian = cv2.GaussianBlur(img, (5, 5), 0)

    cv2.bilateralFilter()

    img = io.imread(r'/Users/memoriuhl/PycharmProjects/OpenCVTesting/wildcat.jpg')
    print(img)
    kernel = np.array([[0, -1, 0], [-1, 5, -1], [0, -1, 0]])
    image_sharpen = cv2.filter2D(img, -1, kernel)
    print('\n First 5 columns and rows of the image_sharpen matrix: \n', image_sharpen[:5, :5] * 255)
    # Adjust the contrast of the filtered image by applying Histogram Equalization
    image_sharpen_equalized = exposure.equalize_adapthist(image_sharpen / np.max(np.abs(image_sharpen)),clip_limit=0.03)

    plt.figure(1)
    plt.axis('off')
    plt.imshow(img), plt.title('Original')

    plt.figure(2)
    plt.axis('off')
    plt.imshow(blur), plt.title('Blur')

    plt.figure(3)
    plt.axis('off')
    plt.imshow(BLUR), plt.title('Edge Mapping')

    plt.figure(4)
    plt.axis('off')
    plt.imshow(image_sharpen_equalized, cmap=plt.cm.gray), plt.title('Sharpened')

    plt.figure(5)
    plt.axis('off')
    plt.imshow(median), plt.title('Median')

    plt.figure(6)
    plt.axis('off')
    plt.imshow(bilat), plt.title('Bilateral')

    plt.show()

cv2_images(img)
