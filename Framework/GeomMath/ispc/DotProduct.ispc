extern void MulByElement(uniform const float a[], uniform const float b[], uniform float result[], uniform const int count);

export void DotProduct(uniform const float a[], uniform const float b[], uniform float* uniform result, uniform const int count) {
	// *result = 0;
	// uniform float * uniform r = uniform new uniform float [count];
	// MulByElement(a, b, r, count);
	// foreach_active(i) {
	// 	*result += r[i];
	// }
	// delete[] r;
	// 作者原实现输出不符合预期，改成以下实现
	varying double sum = 0.0;
    foreach (i = 0 ... count)
        sum += a[i] * b[i];
    *result = reduce_add(sum);
}

