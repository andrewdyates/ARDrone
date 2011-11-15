TESTS=test_gauss test_hsv test_ppm test_mask

all: $(TESTS)

test_mask: mask_test.o threshold.o ppm.o hsv.o util.o gauss.o
	gcc -o test_mask mask_test.o threshold.o ppm.o hsv.o util.o gauss.o
test_gauss: gauss_test.o gauss.o ppm.o
	gcc -o test_gauss gauss_test.o gauss.o ppm.o
test_hsv: ppm.o hsv_test.o hsv.o util.o
	gcc -o test_hsv ppm.o hsv_test.o hsv.o util.o
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
	gcc -c threshold.c

mask_test.o: mask_test.c
	gcc -c mask_test.c
gauss_test.o: gauss_test.c
	gcc -c gauss_test.c
hsv_test.o: hsv_test.c
	gcc -c hsv_test.c
ppm_test.o: ppm_test.c
	gcc -c ppm_test.c

clean:
	rm -f *.o $(TESTS)