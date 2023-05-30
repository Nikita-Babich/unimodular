* The following explanation is shown on 4x4, but that is not a limitation:

a b c d\
e f g h\
i j k l\
m n o p\

* Idea 1 (not active):
All numbers from b to p are set random.
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
The need to calculate determinant makes it O(n^3) in the best implementation.

* Idea 2 (chosen):
Instead of converting a determinant to triagonal form we can generate upper triangular matrix, manipulate its elements on the diagonal so determinant becomes +-1, 
and then make several random row and column additions. That is using the property of determinant:
adding another row/column multiplied by a nonzero number does not change the determinant.
Filling half of the matrix with random values is O(n^2). The amount of random row modifications has to be n, to make the probability of all rows being changed non zero.
O(n^2)+O(n)=O(n^2)