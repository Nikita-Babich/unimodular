* The following explanation is shown on 4x4, but that is not a limitation:

a&nbsp; b&nbsp; c&nbsp; d\
e&nbsp; f&nbsp; g&nbsp; h\
i&nbsp; j&nbsp; k&nbsp; l\
m&nbsp; n&nbsp; o&nbsp; p\


* All numbers from b to p are set random.
"a" will be calculated to fit the equation:

a\*A+-b\*B+c\*C+-d\*D = +-1 \
first row Laplace expansion (https://en.wikipedia.org/wiki/Determinant#Laplace_expansion), where A, B, C, D are minors - determinants of smaller matrices.
Because of the fact that b up to d are already generated, a is the only unknown. \
Everything aside the first component will be calculated and put into

a\*A + R = +-1 \
a\*A = +-1 - R \
a = (+-1 - R) / A \

* How will minors be calculated?
With row reduction the matrix will be converted to upped triagonal form and det will be calculated as multiplication of its diagonal.
Row2 - Row1*(Row2[0]/Row1[0]) guarantees that first element of second row is zero (if machine accuracy limit does not come in).
This operation will be done to each non-first row which does not start with zero (zero does nothing), then it will be done with lower-right submatrix.