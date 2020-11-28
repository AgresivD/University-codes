/*
    * File: Stock.java
        * Description:
            Representing store inventory.
*/
public class Stock
{
    // Defenitions
    final int MAX_STOCK = 100;
    final int DEFAULT_ITEMS = 0;
    final int INVALID_INDEX = -1;
    // Private properties
    private FoodItem [] _stock;
    private int _noOfItems;
    /**
     * Constructor that initializing Stock object by task default values.
    */
    public Stock()
    {
        _stock = new FoodItem[MAX_STOCK];
        _noOfItems = DEFAULT_ITEMS;
    }
    /**
     * Returns the number of active cells in the array.
        * 
        * @return number of active cells
    */
    public int getNumOfItems()
    {
        return _noOfItems;
    }
    /**
     * Adds FoodItem product to inventory.
        * 
        * @param newItem food item object
        * @return true if the add action was successful and false if not.
    */
    public boolean addItem(FoodItem newItem)
    {
        int i = indexOf(_stock, newItem);
        if((i != INVALID_INDEX && (!_stock[i].getProductionDate().equals(newItem.getProductionDate()) || 
            !_stock[i].getExpiryDate().equals(newItem.getExpiryDate()))) || i == INVALID_INDEX)
                addElement(newItem);

        else if(i != INVALID_INDEX && (_stock[i].getProductionDate().equals(newItem.getProductionDate()) && 
            _stock[i].getExpiryDate().equals(newItem.getExpiryDate())))
                _stock[i].setQuantity(_stock[i].getQuantity() + newItem.getQuantity());
        return true;
    }
    /**
     * Returns string text containing a list of product names needed order.
        * 
        * @param amount number of amount to check.
        * @return Returns text containing all products that need to be ordered separated ","
    */
    public String order(int amount)
    {
        String result = new String();
        if(_noOfItems > 0)
        {
            FoodItem[] check = new FoodItem[orderArray().length];
            check = orderArray();
            for(int i = 0; i < _noOfItems; i++)
                if(check[i] != null && check[i].getQuantity() < amount)
                    result += check[i].getName() + ", ";
        }
        int length = result.length();
        return length > 0 ? result.substring(0, length - 2) : result;
    }
    /**
     * Returns the amount of products that can stay in temperature.
        * 
        * @param temp number of refrigerator temperature
        * @return amount of products that can stay in (temp) temperature.
    */
    public int howMany(int temp)
    {
        int result = 0;
        if(_noOfItems > 0)
        {
            for(int i = 0; i < _noOfItems; i++)
                if(temp >= _stock[i].getMinTemperature() && temp <= _stock[i].getMaxTemperature())
                    result += _stock[i].getQuantity();
        }
        return result;
    }
    /**
     * Deletes their expired products on a specific date.
        * 
        * @param d date class object
    */
    public void removeAfterDate(Date d)
    {
        if(_noOfItems > 0)
        {
            for(int i = 0; i < _noOfItems; i++)
                if(_stock[i].getExpiryDate().before(d))
                    removeElement(_stock[i], i);  
        }
    }
    /**
     * Returns the most expensive item in stock.
        * 
        * @return Null if the array is empty otherwise the most expensive FoodItem object
    */
    public FoodItem mostExpensive()
    {
        FoodItem result = null;
        if(_noOfItems > 0)
        {
            for(int i = 0; i < _noOfItems; i++)
                if(result == null || result.getPrice() < _stock[i].getPrice())
                    result = _stock[i];
        }
        return result;
    }
    /**
     * Returns how many items are in stock.
        * 
        * @return number of stock quantity
    */
    public int howManyPieces()
    {
        int result = 0;
        if(_noOfItems > 0)
        {
            for(int i = 0; i < _noOfItems; i++)
                result += _stock[i].getQuantity();
        }
        return result;
    }
    /**
     * Return a string that represents stock products details list.
    */
    public String toString()
    {
        String result = new String();
        if(_noOfItems > 0)
        {
            for(int i = 0; i < _noOfItems; i++)
                result += _stock[i].toString() + "\n";
        }
        return result;
    }
    /**
     * Updates the inventory after the sale,
     * And lowers the quantity according to each product.
        * 
        * @param items String array representing the products sold
    */
    public void updateStock(String[] itemsList)
    {
        if(itemsList.length > 0)
        {
            for(int i = 0; i < itemsList.length; i++)
            {
                int element = indexOf(_stock, itemsList[i]);
                if(element != INVALID_INDEX)
                {
                    int quantity_after = _stock[element].getQuantity() - 1;
                    if(quantity_after <= 0)
                        removeElement(_stock[element], element);
                    else
                        _stock[element].setQuantity(quantity_after);
                }
            }
        }
    }
    /**
     * Returns the temperature at which the refrigerator should be All the food products.
        * 
        * @return The minimum number in range, if not found then return Maximum value of integer type (Integer.MAX_VALUE)
    */
    public int getTempOfStock()
    {
        if(_noOfItems > 0)
        {
            int max_min_temp = _stock[0].getMinTemperature(), min_max_temp = _stock[0].getMaxTemperature();
            for(int i = 1; i < _noOfItems; i++)
            {
                if(_stock[i].getMinTemperature() > max_min_temp)
                    max_min_temp = _stock[i].getMinTemperature();
                if(_stock[i].getMaxTemperature() < min_max_temp)
                    min_max_temp = _stock[i].getMaxTemperature(); 
            }
            for(int i = 0; i < _noOfItems; i++)
                if(!(_stock[i].getMinTemperature() >= max_min_temp || _stock[i].getMaxTemperature() <= min_max_temp))
                    return Integer.MAX_VALUE;
            return (max_min_temp < min_max_temp ? max_min_temp : min_max_temp);
        }
        return Integer.MAX_VALUE;
    }
    /**
     * Return array so that there are no duplicates,
     * Connects the quantities of products if there are duplicates and adjustments to the task conditions.
        * 
        * @return FoodItem array without duplicates.
    */
    private FoodItem[] orderArray()
    {
        if(_noOfItems > 0)
        {
            FoodItem[] temp = new FoodItem[_noOfItems];
            for(int i = 0; i < _noOfItems; i++)
            {
                int element = indexOf(temp, _stock[i]);
                if(element == INVALID_INDEX)
                    temp[i] = _stock[i];
                else if(element != INVALID_INDEX)
                    temp[element].setQuantity(_stock[i].getQuantity() + temp[element].getQuantity());
            }
            return temp;
        }
        return null;
    }
    /**
     * Returns the index of food item object in array,
     * The search for the index also applies to the task rules.
        * 
        * @param array Array to serach on.
        * @param item Food item object to serach.
        * @return (INVALID_INDEX = -1) if item not found, otherwise the index number of the item in array.
    */
    private int indexOf(FoodItem[] array, FoodItem item)
    {
        if(_noOfItems > 0 && array != null)
        {
            for(int i = 0; i < _noOfItems; i++)
                if(array[i] != null && array[i].getName().equals(item.getName()) && array[i].getCatalogueNumber() == item.getCatalogueNumber())
                    return i;
        }
        return INVALID_INDEX;
    }
    /**
     * Returns the index of food item object in array,
     * The search for the index in this method using by food item name.
        * 
        * @param array Array to serach on.
        * @param name Food item name to serach.
        * @return (INVALID_INDEX = -1) if item not found, otherwise the index number of the item in array.
    */
    private int indexOf(FoodItem[] array, String name)
    {
        if(_noOfItems > 0 && array != null)
        {
            for(int i = 0; i < _noOfItems; i++)
                if(array[i] != null && array[i].getName().equals(name))
                    return i;
        }
        return INVALID_INDEX;
    }
    /**
     * Adds a new product to the array.
     * Maintains that there will be no holes in the array,
     * And that the products will be in order of catalog number.
        * 
        * @param item food item object to add.
    */
    private void addElement(FoodItem item) 
    { 
        if (_noOfItems < _stock.length) 
        {
            int i; 
            for (i = _noOfItems - 1; (i >= 0 && _stock[i].getCatalogueNumber() > item.getCatalogueNumber()); i--) 
                _stock[i + 1] = _stock[i]; 
            _stock[i + 1] = item; 
            _noOfItems++;
        }
    }
    /**
     * Removes a product from the array.
     * Maintains that there will be no holes in the array.
        * 
        * @param item food item object to remove.
        * @param index current item index in the array if (INVALID_INDEX = -1) the method will find.
    */
    private void removeElement(FoodItem item, int index)
    {
        int element = index == INVALID_INDEX ? indexOf(_stock, item) : index;
        if(element != INVALID_INDEX)
        {
            for(int i = element + 1; i < _noOfItems; i++)
                _stock[i - 1] = _stock[i];
            _noOfItems--;
        }
    }
}
