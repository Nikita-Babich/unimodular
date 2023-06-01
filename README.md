# Task description
Create a random square matrix with determinant +-1.

# Algorithm
Instead of converting a determinant to triagonal form to see what's the determinant of a given random matrix, we can generate upper triangular matrix, manipulate its elements on the diagonal so determinant becomes +-1, 
and then make several random row or column additions (https://en.wikipedia.org/wiki/Determinant#Example). That is using the property of determinant:
adding another row/column multiplied by a nonzero number does not change the determinant. It is always possible to convert to triangular form without row swaps, so, as we a doing a reverse, we will not use swaps.
We will also not use scaling manipulation, because random filling of the upper triagonal matrix already does the thing of creating any big/small values.
Filling half of the matrix with random values is O(n^2) [exact: (n*(n+1)/2].  

# Proof that we need O(n^2) manipulations total:
Let's look at matrix:\
a b c\
d e f\
g h i\
to convert it to triangular form we need 3 operations.\
Row2 -= Row1 * d/a;\
Row3 -= Row1 * g/a;\
Row3 -= Row2 * h/e;\
The amount of elements under the diagonal (in this case {d, g, h} which we modify to zero) is (n-1)*n/2.
If we convert from a triangular form back to normal matrix, we would need the same amount of operations, to convert each zero to some number. 
But zeros can happen in a matrix and we are not obliged to always get rid of them, so we can choose the amount of operations between 0 and (n-1)*n/2. 
This makes matrices with zeros in lower part a bit more likely, but reduces total computation time by 25% in average.
(n*(n+1)/2 + (n-1)*n/2 = n(n+1+n-1)/2 = n^2 the amount of operations without 25% optimisation.




