/*
    * File: Matrix.java
        * Description:
            Representing a matrix 2D array of integers in the range 0-255.
            
        * @class: Matrix
        * @author Dan Elimelech.
        * @version 1.0, Latest update: Friday, December 27, 2019
*/
public class Matrix
{
    // Definitions
    final int MAX_INT = 255;
    final int MAX_NEIGHBORS = 10;
    // Private properties
    private int[][] arrMatrix;
    /**
     * Constructs a Matrix from a two-dimensional array.
        * 
        * @param array array represnet matrix
    */
    public Matrix(int[][] array)
    {
        arrMatrix = new int[array.length][];
        for(int i = 0; i < array.length; i++)
        {
            arrMatrix[i] = new int[array[i].length];
            for(int j = 0; j < array[i].length; j++)
                arrMatrix[i][j] = (int)array[i][j];
        }
    }
    /**
     * Constructs a size1 by size2 Matrix of zeros.
        * 
        * @param size1 the number of lines
        * @param size2 the number of columns
    */
    public Matrix(int size1, int size2)
    {
        arrMatrix = new int[size1][size2];
        for(int i = 0; i < size1; i++)
            for(int j = 0; j < size2; j++)
                arrMatrix[i][j] = 0;
    }
    /**
     * Returns a string representation of this Matrix.
    */
    public String toString()
    {
        String result = new String();
        for(int i = 0; i < arrMatrix.length; i++)
        {
            for(int j = 0; j < arrMatrix[i].length; j++)
            {
                boolean newLine = (j == arrMatrix[i].length - 1);
                result += arrMatrix[i][j] + (newLine ? "\n" : "\t");
            }
        }
        return result;
    }
    /**
     * Calculates and returns a negative copy of this Matrix.
        * 
        * @return negative copy of this Matrix
    */
    public Matrix makeNegative()
    {
        int[][] result = new int[arrMatrix.length][];
        for(int i = 0; i < arrMatrix.length; i++)
        {
            result[i] = new int[arrMatrix[i].length];
            for(int j = 0; j < arrMatrix[i].length; j++)
                result[i][j] = MAX_INT - arrMatrix[i][j];
        }
        return new Matrix(result);
    }
    /**
     * Calculates and returns a copy of this Matrix after it has been filtered from noise.
        * 
        * @return copy of this Matrix after it has been filtered from noise
    */
    public Matrix imageFilterAverage() 
    {
        int[][] result = new int[arrMatrix.length][];
        for(int i = 0; i < arrMatrix.length; i++)
        {
            result[i] = new int[arrMatrix[i].length];
            for(int j = 0; j < arrMatrix[i].length; j++)
                result[i][j] = averageOf(neighborsOf(i, j));
        }
        return new Matrix(result);
    }
    /**
     * Rotating the image 90 degrees clockwise.
        * 
        * @return copy of this Matrix after clockwise rotating
    */
    public Matrix rotateClockwise()
    {
        Matrix matrix = rotateCounterClockwise();
        return matrix.rotateCounterClockwise().rotateCounterClockwise();
    }
    /**
     * Rotating the image 90 degrees against clockwise.
     * 
     * @return copy of this Matrix after against clockwise rotating
    */
    public Matrix rotateCounterClockwise()
    {
        int[][] result = new int[arrMatrix[0].length][arrMatrix.length];
        for(int i = 0; i < arrMatrix.length; i++)
            for(int j = arrMatrix[i].length - 1; j >= 0; j--)
                result[arrMatrix[i].length - 1 - j][i] = arrMatrix[i][j];
        return new Matrix(result);
    }
    /**
     * Fetch the neighbros of a given (x,y),
     * Returns array of neighbors for a given index.
        * 
        * @param x_index index of x
        * @param y_index index of y
        * @return array of neighbors for given (x, y)
    */
    private int[] neighborsOf(int x_index, int y_index) 
    {
        int[] tmpArray = new int[MAX_NEIGHBORS];
        int counter = 0;
        for (int x = 0; x < arrMatrix.length; x++)
            for (int y = 0; y < arrMatrix[x].length; y++)
                if (Math.abs(x_index - x) <= 1 && 1 >= Math.abs(y_index - y))
                    tmpArray[counter++] = arrMatrix[x][y];   
        int[] result = new int[counter];
        for (int i = 0; i < counter; i++)
            result[i] = tmpArray[i];     
        return result;
    }
    /**
     * Calculate the avarage of int array.
        * 
        * @param array calculate array
        * @return returns the array avarage.
    */
    private int averageOf(int[] array)
    {
        int result = 0;
        for(int i = 0; i < array.length; i++)
            result += array[i];
        return result / array.length;
    }
}
