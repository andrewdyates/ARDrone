TESTS=test_gauss test_hsv test_ppm test_mask test_util test_filter

all: $(TESTS)

test_filter: filter_test.o ppm.o binfilter.o
	gcc -o test_filter filter_test.o ppm.o binfilter.o
test_mask: mask_test.o threshold.o ppm.o hsv.o util.o gauss.o moments.o 
	gcc -o test_mask mask_test.o threshold.o ppm.o hsv.o util.o gauss.o moments.o 
test_gauss: gauss_test.o gauss.o ppm.o
	gcc -o test_gauss gauss_test.o gauss.o ppm.o
test_hsv: ppm.o hsv_test.o hsv.o util.o
	gcc -o test_hsv ppm.o hsv_test.o hsv.o util.o
test_ppm: ppm.o ppm_test.o
	gcc -o test_ppm ppm.o ppm_test.o
test_util: util.o util_test.o
	gcc -o test_util util.o util_test.o

moments.o: moments.c
	gcc -c -O3 moments.c
gauss.o: gauss.c
	gcc -c -O3 gauss.c
hsv.o: hsv.c
	gcc -c -O3 hsv.c
ppm.o: ppm.c
	gcc -c -O3 ppm.c
util.o: util.c
	gcc -c -O3 util.c
threshold.o: threshold.c
	gcc -c -O3 threshold.c
binfilter.o: binfilter.c
	gcc -c -O3 binfilter.c

filter_test.o: filter_test.c
	gcc -c -O3 filter_test.c
mask_test.o: mask_test.c
	gcc -c -O3 mask_test.c
gauss_test.o: gauss_test.c
	gcc -c -O3 gauss_test.c
hsv_test.o: hsv_test.c
	gcc -c -O3 hsv_test.c
ppm_test.o: ppm_test.c
	gcc -c -O3 ppm_test.c
util_test.o: util_test.c
	gcc -c -O3 util_test.c

clean:
	rm -f *.o $(TESTS)