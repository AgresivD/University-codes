/*
    * File: PolyNode.java
        * Description:
            Represents one polynomial organ.
            
        * @class: PolyNode
        * @author Dan Elimelech.
        * @version 1.0, Latest update: Saturday, Febuary 8, 2020
*/
public class PolyNode
{
    private int _power;
    private double _coefficient;
    private PolyNode _next;
    /**
         * Copy constructor.
            * @param p PolyNode object.
    */
    public PolyNode(PolyNode p)
    {
        this(p.getPower(), p.getCoefficient(), p.getNext());
    }
    /**
         * Constructor that accepts power and coefficient, next will initialized to null.
            * @param power power of the polynomial.
            * @param coefficient coefficient of the polynomial.
    */
    public PolyNode (int power, double coefficient)
    {
        _power = power < 0 ? 0 : power;
        _coefficient = power < 0 ? 0 : coefficient;
        _next = null;
    }
    /**
         * Constructor that accepts three properties of PolyNode and initialize them.
            * @param power power of the polynomial.
            * @param coefficient coefficient of the polynomial.
            * @param next PolyNode object.
    */
    public PolyNode(int power, double coefficient, PolyNode next)
    {
        this(power, coefficient);
        _next = next;
    }
    /**
         * Gets the power.
            * @return Integer number that represent the power. 
    */
    public int getPower()
    {
        return _power;
    }
    /**
         * Updates power property of the object.
            * @param power power of the polynomial.
    */
    public void setPower(int power)
    {
        if(power >= 0)
            _power = power;
    }
    /**
         * Gets the coefficient.
            * @return Double type number that represent the coefficient. 
    */
    public double getCoefficient()
    {
        return _coefficient;
    }
    /**
         * Updates power property of the object.
            * @param coefficient coefficient of the polynomial.
    */
    public void setCoefficient(double coefficient)
    {
        _coefficient = coefficient;
    }
    /**
        * Gets next pointer.
            * @return PolyNode object that represent the next object of this.
    */
    public PolyNode getNext()
    {
        return _next;
    }
    /**
         * Updates next property of the object.
            * @param next PolyNode object.
    */
    public void setNext(PolyNode next)
    {
        _next = next;
    }
    /**
        * Returns string representation of the polynomial.
    */
    public String toString()
    {
        if(_coefficient == 0)
            return "";
        else if(_power == 0)
            return String.valueOf(_coefficient);
        else
        {
            if(_coefficient == 1 || _coefficient == -1)
                return (_coefficient < 0 ? "-x" : "x") + (_power == 1 ? "" : "^" + _power);
            else
                return _coefficient + "x" + (_power == 1 ? "" : "^" + _power);
        }   
    }
}
