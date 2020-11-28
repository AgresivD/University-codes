/**
    * Representing food products at store.
*/
public class FoodItem
{
    // Defenitions of invalid fooditem properties
    private final String DEFAULT_EMPTY_TITLE = "item";
    private final int DEFAULT_QUANTITY = 0;
    private final int DEFAULT_PRICE = 1;
    // Declare private variables that properties food item product.
    private String name;
    private long catalogueNumber;
    private int quantity;
    private Date productionDate;
    private Date expiryDate;
    private int minTemperature;
    private int maxTemperature;
    private int price;
    /**
     * Constructor that initializing FoodItem object by property values.
        * 
        * @param name product name
        * @param catalogueNumber product catalogue id 
        * @param quantity quantity of this product
        * @param productionDate Date object representing the product's production date
        * @param expiryDate Date object representing the validity of the product
        * @param minTemperature number of the minimum temperature for this product
        * @param maxTemperature number of the maximum temperature for this product
        * @param price price of the product
    */
    public FoodItem(String name, long catalogueNumber, int quantity, Date productionDate, Date expiryDate, int minTemperature, int maxTemperature, int price)
    {
        this.name = name == "" ? DEFAULT_EMPTY_TITLE : name; 
        this.expiryDate = expiryDate.before(productionDate) ? productionDate.tomorrow() : expiryDate;
        this.productionDate = productionDate;
        this.minTemperature = minTemperature > maxTemperature ? maxTemperature : minTemperature;
        this.maxTemperature = this.minTemperature == minTemperature ? maxTemperature : minTemperature;
        this.catalogueNumber = (catalogueNumber < 0 || String.valueOf(catalogueNumber).length() != 4) ? 9999 : catalogueNumber;
        this.quantity = quantity < 0 ? DEFAULT_QUANTITY : quantity;
        this.price = price < 0 ? DEFAULT_PRICE : price;
    }
    /**
     * Constructor that initializing FoodItem object by another exsisting.
        * 
        * @param other food item object
    */
    public FoodItem(FoodItem other)
    {
        this(other.getName(), other.getCatalogueNumber(), other.getQuantity(), other.getProductionDate(), other.getExpiryDate(), other.getMinTemperature(),
            other.getMaxTemperature(), other.getPrice());
    }
    /**
     * Gets the catalogue number.
        * 
        * @return 4 digits number that represnet the catalogue id
    */
    public long getCatalogueNumber()
    {
        return catalogueNumber;
    }
    /**
     * Gets the product name.
        * 
        * @return string that represnet the item name.
    */
    public String getName()
    {
        return name;
    }
    /**
     * Gets the product quantity.
        * 
        * @return number that represnet the quantity of this item
    */
    public int getQuantity()
    {
        return quantity;
    }
    /**
     * Gets the date of item production.
        * 
        * @return date object that represnet the date of item production
    */
    public Date getProductionDate()
    {
        return productionDate;
    }
    /**
     * Gets the date of item validity.
        * 
        * @return date object that represnet the date of item expiry
    */
    public Date getExpiryDate()
    {
        return expiryDate;
    }
    /**
     * Gets the minmum temprature of item.
        * 
        * @return number that represent the minimum temprature of item
    */
    public int getMinTemperature()
    {
        return minTemperature;
    }
    /**
     * Gets the maximum temprature of item.
        * 
        * @return number that represent the maximum temprature of item
    */
    public int getMaxTemperature()
    {
        return maxTemperature;
    }
    /**
     * Gets the price of item.
        * 
        * @return number that represent how much cost this item
    */
    public int getPrice()
    {
        return price;
    }
    /**
     * Set the quantity (only if not negative).
        * 
        * @param n posetive number of item quantity
    */
    public void setQuantity(int n)
    {
        if(n >= 0)
            quantity = n;
    }
    /**
     * Set the production date of item by exsisting date object.
        * 
        * @param d date class object
    */
    public void setProductionDate(Date d)
    {
        if(!expiryDate.before(d))
            productionDate = d;
    }
    /**
     * Set the validty of item by exsisting date object.
        * 
        * @param d date class object
    */
    public void setExpiryDate(Date d)
    {
        if(expiryDate.before(d))
            expiryDate = d;
    }
    /**
     * Set the price of item.
        * 
        * @param n posetive number of item price
    */
    public void setPrice(int n)
    {
        if(n >= 0)
            price = n;
    }
    /**
     * Check if 2 food items are the same (excluding the quantity values)
        * 
        * @param other the food item to compare this food item to
        * @return true if the food items are the same
    */
    public boolean equals(FoodItem other)
    {
        return name.equals(other.getName()) && catalogueNumber == other.getCatalogueNumber() &&
            productionDate.equals(other.getProductionDate()) && expiryDate.equals(other.getExpiryDate()) && minTemperature == other.getMinTemperature() &&
            maxTemperature == other.getMaxTemperature() && price == other.getPrice();
    }
    /**
     * Check if this food item is fresh on the date d.
        * 
        * @param d date to check
        * @return true if this food item is fresh on the date d
    */
    public boolean isFresh(Date d)
    {
        return d.before(expiryDate);
    }
    /**
     * Return a string that represents this food item properties.
    */
    public String toString()
    {
        return "\n\t\t--- FoodItem: " + name + " ---\n\n\t CatalogueNumber: " + catalogueNumber + "\n\t ProductionDate: " + 
            productionDate.toString() + "\n\t ExpiryDate: " + expiryDate.toString() + "\n\t Quantity: " + this.quantity + 
                "\n\t Min Temprature: " + minTemperature + "\n\t Max Temprature: " + maxTemperature + "\n\t Price: " + price;
    }
    /**
     * Check if this food item is older than other food item.
        * 
        * @param other food item to compare this food item to
        * @return true if this food item is older than other date
    */
    public boolean olderFoodItem(FoodItem other)
    {
        return productionDate.before(other.getProductionDate());
    }
    /**
     * Returns the number of units of products that can be purchased for a given amount.
        * 
        * @param amount amount to purchase
        * @return the number of units can be purchased
    */ 
    public int howManyItems(int amount)
    {
        int items = amount / price;
        if(quantity >= items)
            return items;
        return quantity;
    }
    /**
     * Check if this food item is cheaper than other food item.
     * @param other food item to compare this food item to
     * @return true if this food item is cheaper than other date
    */
    public boolean isCheaper(FoodItem other)
    {
        return (price < other.getPrice());
    }
}
