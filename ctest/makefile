TESTS=test_gauss test_hsv test_ppm
GAUSS=gauss.o ppm.o
HSV=hsv.o util.o

all: $(TESTS)

test_gauss: gauss_test.o $(GAUSS)
	gcc -o test_gauss gauss_test.o $(GAUSS)
test_hsv: ppm.o hsv_test.o $(HSV)
	gcc -o test_hsv ppm.o hsv_test.o $(HSV)
test_ppm: ppm.o ppm_test.o
	gcc -o test_ppm ppm.o ppm_test.o

gauss.o: gauss.c
	gcc -c gauss.c
hsv.o: hsv.c
	gcc -c hsv.c
ppm.o: ppm.c
	gcc -c ppm.c
util.o: util.c
	gcc -c util.c
threshold.o: threshold.c
	gcc -c util.c

gauss_test.o: gauss_test.c
	gcc -c gauss_test.c
hsv_test.o: hsv_test.c
	gcc -c hsv_test.c
ppm_test.o: ppm_test.c
	gcc -c ppm_test.c

clean:
	rm -f *.o $(TESTS)