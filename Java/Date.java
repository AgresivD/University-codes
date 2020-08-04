/** 
    * File: Date.java
        * Description:
            Represents a date using its individual attributes (month, day, year)
            
        * @class: Date
        * @author Dan Elimelech.
        * @date Friday, November 16, 2019
 */
public class Date
{
    // Defenitions of the invalid dates properties.
    final int DEFAULT_INVALID_DAY = 1;
    final int DEFAULT_INVALID_MONTH = 1;
    final int DEFAULT_INVALID_YEAR = 2000;
    // Defenitions of date setting limits.
    final int MIN_DAY = 1;
    final int MAX_DAY = 31;
    final int MIN_MONTH = 1;
    final int MAX_MONTH = 12;
    final int MAX_YEAR_DIGITS = 4;
    // Defenitions usage types of private function called validDate.
    final int DAY_CHECK_TYPE = 0;
    final int MONTH_CHECK_TYPE = 1;
    final int YEAR_CHECK_TYPE = 2;
    // Create internal variables that's repersents a date.
    private int _day;
    private int _month;
    private int _year;
    /**
    * Constructor initialized by Date property values.
         *
         * @param day number of the day.
         * @param month number of the month.
         * @param year number of the year.
    */
    public Date(int day, int month, int year)
    {
        if(validDate(day, month, year))
            setProperties(day, month, year);
        else
            setProperties(DEFAULT_INVALID_DAY, DEFAULT_INVALID_MONTH, DEFAULT_INVALID_YEAR);
    }
    /**
    * Constructor initilaized by Date object.
         *
         * @param other date object.
    */
    public Date(Date other)
    {
        this(other.getDay(), other.getMonth(), other.getYear());
    }
    /**
    * Update date property of day value.
         *
         * @param dayToSet number of the day.
    */
    public void setDay(int dayToSet)
    {
        // verify that day is correct value and date will be correct with this day value.
        if(validDate(dayToSet, DAY_CHECK_TYPE) && validDate(dayToSet, _month, _year))
            _day = dayToSet;
    }
    /**
    * Return the current date property value of day.
         *
         * @return int type number that represent the day number of date object.
    */
    public int getDay()
    {
        return _day;
    }
    /**
    * Update date property of month value.
         *
         * @param monthToSet number of the month.
    */
    public void setMonth(int monthToSet)
    {
        if(validDate(monthToSet, MONTH_CHECK_TYPE) && validDate(_month, monthToSet, _year))
            _month = monthToSet;
    }
    /**
    * Return the current date property value of month.
         *
         * @return int type number that represent the month number of date object.
    */
    public int getMonth()
    {
        return _month;
    }
    /**
    * Update date property of year value.
         *
         * @param yearToSet number of the year.
    */
    public void setYear(int yearToSet)
    {
        if(validDate(yearToSet, YEAR_CHECK_TYPE) && validDate(_day, _month, yearToSet))
            _year = yearToSet;
    }
    /**
    * Return the current date property value of year.
         *
         * @return int type number that represent the year number of date object
    */
    public int getYear()
    {
        return _year;
    }
    /**
    * Check if the dates are equal
         *
         * @param other date to compare
         * @return true if the dates are the same
    */
    public boolean equals(Date other)
    {
        return (other.getDay() == _day && other.getMonth() == _month && other.getYear() == _year);
    }
    /**
    * Check if this date is before date parameter.
         *
         * @param other date to compare this date to
         * @return true if this date is before other date
    */
    public boolean before(Date other)
    { 
        if(_year < other.getYear())
            return true;
        else if(_year == other.getYear() && (_month < other.getMonth()))
            return true;
        else if((_year == other.getYear() && (_month == other.getMonth())) && (_day < other.getDay()))
            return true;
        return false;
    }
    /**
    * Check if this date is after other date.
         *
         * @param other date to compare this date to
         * @return true if this date is after other date
    */
    public boolean after(Date other)
    {
        return !(before(other));
    }
    /**
    * Calculates the difference in days between two dates.
         *
         * @param other the date to calculate the difference between
         * @return the number of days between the dates
    */
    public int difference(Date other)
    {
        int result = calculateDate(_day, _month, _year) - calculateDate(other.getDay(), other.getMonth(), other.getYear());
        if(result >= 0)
            return result;
        return 0;
    }
    /**
    * Calculate the date of tomorrow.
         *
         * @return the date of tomorrow
    */
    public Date tomorrow()
    {
        if((_day == MAX_DAY) && (_month != 4 && _month != 6 && _month != 9 && _month != 11))
            return (new Date(1, _month == 12 ? 1 : _month + 1, _month == 12 ? _year + 1 : _year));
        else if((_day == MAX_DAY - 1) && (_month == 4 || _month == 6 || _month == 9 || _month == 11))
            return (new Date(1, _month + 1, _year));
        else if(_month == 2 && (isLeapYear(_year) && _day == 29 || !isLeapYear(_year) && _day == 28))
            return (new Date(1, 3, _year));
        return (new Date(_day + 1, _month, _year));
    }
    /**
    * Calculate the day of the week that this date occurs on 0-Saturday 1-Sunday 2-Monday etc.
         *
         * @return the day of the week that this date occurs on
    */
    public int dayInWeek()
    {
        int month = _month;
        int year = _year;
        if(month < 3)
        {
            year--;
            month += 12;
        }
        int last = Integer.valueOf(String.valueOf(year).substring(String.valueOf(year).length() - 2));
        int first = Integer.valueOf(String.valueOf(year).substring(0, 2));
        int result = (_day + (26 * (month + 1)) / 10 + last + (last / 4) + (first / 4) - 2 * first) % 7;
        return result < 0 ? Math.floorMod(result, 7) : result;
    }
    /**
    * Return a string that represents this date.
         *
         * @return string that represents this date in the following format: day/month/year for example: 02/03/1998
    */
    public String toString()
    {
        return format(String.valueOf(_day)) + "/" + format(String.valueOf(_month)) + "/" + String.valueOf(_year);
    }
    /** 
    * Return the string so that added zero if parameter contains only one number.
         *
         * @param value number to convert as string
         * @return string that represents the formated number.
    */
    private String format(String value)
    {
        if(value.length() == 1)
            return "0" + value;
        return value;
    }
    /** 
    * Validate that three values day, month, year are valid for a date and can together represent correct date.
         *
         * @param day number of the day.
         * @param month number of the month.
         * @param year number of the year.
         * @return true if date properties are valid and date is correct.
    */
    private boolean validDate(int day, int month, int year)
    {
        // check with second usage of validDate function if all values is correct for date.
        if(validDate(day, DAY_CHECK_TYPE) && validDate(month, MONTH_CHECK_TYPE) && validDate(year, YEAR_CHECK_TYPE))
        {
            // possibility that a day on the date will be 31 and the month is invalid
            if((day == MAX_DAY) && (month == 4 || month == 6 || month == 9 || month == 11))
                return false;
            // possibility that the date is leap year and his day is over 29 (maximum in this case)
            else if((isLeapYear(year) && month == 2 && day > 29) || (!isLeapYear(year) && month == 2 && day > 28))
                return false;
            return true;
        }
        return false;
    }
    /** 
    * Validate that number and the type of validation check is correct.
         *
         * @param value number to check.
         * @param type validation check type (Day Check = 0, Month Check = 1, Year Check = 2)
    */
    private boolean validDate(int value, int type)
    {
        switch(type)
        {
            // return true if value is correct day
            case DAY_CHECK_TYPE:
                return (value >= MIN_DAY && value <= MAX_DAY);
            // return true if value is correct month
            case MONTH_CHECK_TYPE:
                return (value >= MIN_MONTH && value <= MAX_MONTH);
            // return true if value is correct year
            case YEAR_CHECK_TYPE:
                return (value > 0 && String.valueOf(value).length() == MAX_YEAR_DIGITS);
        }
        return false;
    }
    /** 
    * Accepts year number and return true if is leap year.
         *
         * @param year year number to check
         * @return true if year is leap
    */
    private boolean isLeapYear(int year)
    {
        if((year % 4) == 0)
        {
            if((year % 100) == 0)
                return (year % 400) == 0;
            return true;
        }
        return false;
    }
    /** 
    * Initilaize Date object properties.
         *
         * @param day number of the day.
         * @param month number of the month.
         * @param year number of the year.
    */
    private void setProperties(int day, int month, int year)
    {
        _day = day;
        _month = month;
        _year = year;
    }
    /** 
    * Computes the day number since the beginning of the Christian counting of years.
         *
         * @param day number of the day.
         * @param month number of the month.
         * @param year number of the year.
         * @return number days passed from the beginning of the Christian counting of years.
    */
    private int calculateDate(int day, int month, int year) 
    {
        if (month < 3) 
        {
            year--;
            month = month + 12;
        }
        return 365 * year + year/4 - year/100 + year/400 + ((month+1) * 306)/10 + (day - 62);
    } 
}
