# Task description
Create a random square matrix with determinant +-1.

# Algorithm
Instead of converting a determinant to triagonal form to see what's the determinant of a given random matrix, we can generate upper triangular matrix, manipulate the last element on the diagonal so the determinant becomes +-1, 
and then make several random row or column additions (https://en.wikipedia.org/wiki/Determinant#Example). That is using the property of determinant:
adding another row/column multiplied by a nonzero number does not change the determinant. It is always possible to convert to triangular form without row swaps, so, as we a doing a reverse, we will not use swaps. 
The swaps can be applied just in the end to solve the issue: inability for zeros to appear on the main diagonal.
We will not use scaling manipulation, because random filling of the upper triagonal matrix already does the thing of creating any big/small values.
Filling half of the matrix with random values is O(n^2) [exact: n(n+1)/2].  

# Proof that we need n^2 manipulations total:
Let's look at matrix:\
a b c\
d e f\
g h i\
to convert it to triangular form we need 3 operations.\
Row2 -= Row1 * d/a;\
Row3 -= Row1 * g/a;\
Row3 -= Row2 * h/e;\
The amount of elements under the diagonal (in this case {d, g, h} which we modify to zero) is (n-1)n/2.
If we convert from a triangular form back to normal matrix, we would need the same amount of operations, to convert each zero to some number. 
But zeros can happen in a matrix and we are not obliged to always get rid of them, so we can choose the amount of operations between 0 and (n-1)n/2. 
This makes matrices with zeros in lower part a bit more likely, but reduces total computation time by 25% in average.\
n(n+1)/2 + (n-1)n/2 = n(n+1+n-1)/2 = n^2 the amount of manipulations without 25% optimisation.\
But row manipulation is n simple operations on numbers.\
n(n+1)/2 + n(n-1)n/2 = n(n+1+n^2-1)/2 = (n^2+n^3)/2.\
# Total time complexity: O(n^3)

# How many swaps is needed?
How many swaps is needed to go from any permutation of row to any other permutation?\
If i swap row indices, it would take n for shuffling and n^2 for filling a new shuffled matrix.\
n + n^2 << n^3 so it does not change the total time complexity.

For the swap I need "pick 2 from n" random number generator. Or a whole array of indices shuffle. I need not the amount of swaps to be even.

# Example.
Given number 3 we will fill the upper part of matrix 3x3 with random floats.\
0 0.1 -0.3\
0 0 1.5\
0 0 0\
Then the diagonal will be filled with random nonzero numbers, except the last one, which is set to make the diagonal product =1.\
2 0.1 -0.3\
0 2 1.5\
0 0 0.25\
After that up to (n-1)n/2 row operations. Let's say there were just 2 operations (3)+=(1) and (2)+=2(3).
2 0.1 -0.3\
0 2 1.5\
2 0.1 -0.05\
-----------\
2 0.1 -0.3\
4 2.2 1.4\
2 0.1 -0.05\
And a row swap\
2 0.1 -0.3\
2 0.1 -0.05\
4 2.2 1.4\
It can be checked, that the resulting matrix had a det=-1.\

# Current problems
Indices 0,3 an 3,3 create zero elements instead of random.
Sometimes segfaults.
First couple elements of the diagonal are so big, that the 1/fabs(accumulated_value) is rounded down to 0.



