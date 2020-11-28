/*
    * File: Ext14.java
        * Description:
            Representing Maman 14.
*/
public class Ex14
{
    // Declare constant variables.
    static final int DEFAULT_CLOSET = -1;
    static final int DEFAULT_SUBSTRC = 1;
    /**
     * Receives a string s and a character 'c' and returns 
     * How many sub strings there are in s that start and end with a character 'c' using subStrMaxC overloading.
     * Complications: O(n)
        * @param s String to look.
        * @param c Input char to look by.
        * @return The numner of sub strings found.
    */
    public static int subStrC(String s, char c)
    {
        return subStrMaxC(s, c, DEFAULT_SUBSTRC);
    }
    /**
     * Receives a string s, a character 'c', and a positive integer k, and returns 
     * How many sub strings there are in s that begin and
     * end with a character 'c' and have a maximum k times the character 'c'.
     * Complications: O(n)
        * @param s String to look.
        * @param c Input char to look by.
        * @param k Amount of chars inside the pattern.
        * @return The numner of sub strings found.
    */
    public static int subStrMaxC(String s, char c, int k)
    {
        int counter = 0;
        for(int i = 0; i < s.length(); i++)
            if(s.charAt(i) == c)
                counter++;
        return subStrMaxCRecursive(k, counter);
    }
    private static int subStrMaxCRecursive(int k, int counter)
    {
        if(k < 0) // our exit condition.
            return 0;
        if(counter < k + 2)
            counter = 0;
        else
            counter -= k + 1;
        return counter + subStrMaxCRecursive(k - 1, counter);
    }
    /**
     * Replaces any value with a non-zero array From it to the nearest zero point (right or left).
     * Complications: O(n)
        * @param a Array to replace.
    */
    public static void zeroDistance(int[] a)
    {
        int closet = DEFAULT_CLOSET;
        for(int i = 0; i < a.length; i++)
            if(a[i] == 0)
                closet = 0;
            else
                a[i] = closet == DEFAULT_CLOSET ? Integer.MAX_VALUE : ++closet;       
        closet = DEFAULT_CLOSET;
        for(int i = a.length - 1; i >= 0; i--)
            if(a[i] == 0)
                closet = 0;
            else if(closet != DEFAULT_CLOSET && a[i] > ++closet)
                a[i] = closet;
    }
    /**
     * Checks if all string s unique chars are a subset of string t.
        * @param s Character string.
        * @param t String transformed from character string s.
        * @return True if t has been transformed from the string s, and false otherwise.
    */
    public static boolean isTrans(String s, String t)
    {
        if ((s == "" || t == "") && s != t)
            return false;
        if (s.equals(t))
            return true;
        if (t.length() < s.length())
            return false;
        return isTransRecursive(s, t);       
    } 
    // Recursive helper function for isTrans.
    private static boolean isTransRecursive(String s, String t)
    {
        if (t.length() < s.length())
            return false;
        if (s.length() == 0)
            return true;
        if (s.charAt(0) == t.charAt(0))
            return isTransRecursive(s.substring(1), t.substring(1));
        else if (s.charAt(0) != t.charAt(0))
            return isTransRecursive(s, t.substring(1));
        return false;
    }
    /**
     * Accepts a two-dimensional int[][] array as the parameter and searches for legal "paths" through it
     * Starting at [0][0] and ending at the end of the array.
     * @param a Two-dimensional integer array.
     * @return Returns the number of possible valid paths in the array.
    */
    public static int countPaths(int[][] a) 
    {
        int start = a[0][0], tens = start / 10, ones = start % 10;
        boolean downByOnes = (ones <= a.length - 1 && tens <= a[ones].length - 1);
        if((tens <= a.length - 1 && ones <= a[tens].length - 1)) 
            if((ones <= a.length - 1 && tens <= a[ones].length - 1)) 
                return countPaths(a, tens, ones) + countPaths(a, ones, tens);
            else 
                return countPaths(a, tens, ones);
        else 
            if(downByOnes)
                return countPaths(a, ones, tens);
            else
                return 0;
    }
    private static int countPaths(int[][] a, int row, int col) 
    {
        int current = a[row][col], tens = current / 10, 
            ones = current % 10, end = a[a.length - 1][a[0].length - 1];
        boolean downByOnes = (row + ones <= a.length - 1 && col + tens <= a[row + ones].length - 1);
        if(current == end) 
            return 1;
        else 
            if(row + tens <= a.length - 1 && col + ones <= a[row + tens].length - 1) 
                if(downByOnes) 
                    return countPaths(a, row + tens, col + ones) + countPaths(a, row + ones, col + tens);
                else
                    return countPaths(a, row + tens, col + ones);
            else 
                if(downByOnes)
                    return countPaths(a, row + ones, col + tens);
                else
                    return 0;
    }
}
