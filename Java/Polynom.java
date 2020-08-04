/*
    * File: Polynom.java
        * Description:
            Represents a polynomial using a linked list.
            
        * @class: Polynom
        * @author Dan Elimelech.
        * @version 1.0, Latest update: Saturday, Febuary 8, 2020
*/
public class Polynom
{
    private PolyNode _head;
    /**
        * Default constructor initilaize head to be null.
    */
    public Polynom()
    {
        _head = null;
    }
    /**
         * Copy Constructor.
            * @param p PolyNode object.
    */
    public Polynom(PolyNode p)
    {
        _head = p;
    }
    /**
         * Gets the pointer to head of this object.
            * @return PolyNode object that represent the head.
    */
    public PolyNode getHead()
    {
        return _head;
    }
    /**
        * Updates the head of the linked list.
            * @param head PolyNode object.
    */
    public void setHead(PolyNode head)
    {
        _head = head;
    }
    /**
         * Add PolyNode object by power oreder from high to low to linked list.
            * @param p PolyNode object.
            * @return Returns the new updated polynom object.
    */
    public Polynom addNode(PolyNode p)
    {
        PolyNode tmp = _head;
        while(tmp != null)
        {
            if(tmp.getPower() == p.getPower())
            {
                tmp.setCoefficient(tmp.getCoefficient() + p.getCoefficient());
                return this;
            }
            tmp = tmp.getNext();
        }
        p.setNext(_head);
        setHead(mergeSort(p));
        return this;
    }
    /**
         * Multiplies the polynomial in scalar.
            * @param num integer scalar.
            * @return returns the The new polynomial.
    */
    public Polynom multByScalar(int num)
    {
        PolyNode tmp = _head;
        while(tmp != null)
        {
            tmp.setCoefficient(tmp.getCoefficient() * num);
            tmp = tmp.getNext();
        }
        return this;
    }
    /**
         * Adds a polynomial to the polynomial on which this method is applied.
            * @param other Polynom object.
            * @return Returns the sum polynomial.
    */
    public Polynom addPol(Polynom other)
    {
        PolyNode tmp = other.getHead();
        while(tmp != null)
        {
            addNode(tmp);
            tmp = tmp.getNext();
        }
        return this;
    }
    /**
         * Accepts as a polynomial parameter and multiplies it 
         * by the polynomial on which the method is applied.
            * @param other Polynom object.
            * @return Returns the polynomial multiplication.
    */
    public Polynom multPol(Polynom other)
    {
        if(other.getHead() == null || getHead() == null)
            return this;
        else
        {
            Polynom result = new Polynom();
            PolyNode p1 = getHead();
            while(p1 != null)
            {
                PolyNode p2 = other.getHead();
                while(p2 != null)
                {
                    result.addNode(new PolyNode(p2.getPower() + p1.getPower(), p2.getCoefficient() * p1.getCoefficient()));
                    p2 = p2.getNext();
                }
                p1 = p1.getNext();
            }
            this.setHead(result.getHead());           
            return this;
        }
    }
    /**
        * Calculates the derivative of the polynomial.
            * @return Returns the derived polynomial.
    */
    public Polynom differential()
    {
        PolyNode tmp = getHead();
        Polynom result = new Polynom();
        while(tmp != null)
        {
            if(tmp.getPower() == 1)
                tmp.setPower(0);
            if(tmp.getPower() != 0)
                result.addNode(tmp.getPower() > 1 ? (new PolyNode(tmp.getPower() - 1, tmp.getCoefficient() * tmp.getPower())) : tmp);
            tmp = tmp.getNext();
        }
        setHead(result.getHead());
        return this;
    }
    /**
     * Returns string representation of the polynomial.
    */
    public String toString()
    {
        String result = "";
        PolyNode tmp = getHead();
        while(tmp != null)
        {
            if(tmp.getCoefficient() > 0)
                result += ((tmp != getHead()) ? "+" : "") + tmp.toString();
            else
                result += tmp.toString();
            tmp = tmp.getNext();
        }
        return result;
    }
    /**
        * Merge the sorted a and b.
            * @param a list a.
            * @param b list b.
            * @return Returns linked list are contains the merge of a and b by order of power property.
    */
    private PolyNode sortedMerge(PolyNode a, PolyNode b)
    {
        if(a == null || b == null)
            return a == null ? b : a;
        PolyNode result = null;
        if(a.getPower() >= b.getPower())
        {
            result = a;
            result.setNext(sortedMerge(a.getNext(), b));
        }
        else
        {
            result = b;
            result.setNext(sortedMerge(a, b.getNext()));
        }
        return result;
    }
    /**
         * Gets middle object of Polynom linked list.
            * @param head PolyNode object of head.
            * @return Returns PolyNode object that represent the middle object of the head linked list.
    */
    private PolyNode getMiddle(PolyNode head)
    {
        if(head == null)
            return head;
        PolyNode slow = head, fast = head;
        while(fast.getNext() != null && fast.getNext().getNext() != null)
        {
            slow = slow.getNext();
            fast = fast.getNext().getNext();
        }
        return slow;
    }
    /**
         * Merged sorting to linked list from the big to the small by looking the power.
            * @param head PolyNode object of head.
            * @return Returns PolyNode object that represent the head of merge sorting result.
    */
    private PolyNode mergeSort(PolyNode head)
    {
        if(head == null || head.getNext() == null)
            return head;
        PolyNode middle = getMiddle(head);
        PolyNode nextMiddle = middle.getNext();
        middle.setNext(null);
        PolyNode left = mergeSort(head),
            right = mergeSort(nextMiddle);
        return sortedMerge(left, right);  
    }
}
