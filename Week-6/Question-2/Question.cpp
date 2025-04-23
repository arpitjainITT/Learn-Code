// Look at the below classes and the client code given below on how the object are used and methods invoked. Is there a better way to write the Customer class?

public 
class Customer
{

private
    String firstName;

private
    String lastName;

private
    Wallet myWallet;

public
    String getFirstName()
    {

        return firstName;
    }

public
    String getLastName()
    {

        return lastName;
    }

public
    Wallet getWallet()
    {

        return myWallet;
    }

}

public class Wallet
{

private
    float value;

public
    float getTotalMoney()
    {

        return value;
    }

public
    void setTotalMoney(float newValue)
    {

        value = newValue;
    }

public
    void addMoney(float deposit)
    {

        value += deposit;
    }

public
    void subtractMoney(float debit)
    {

        value -= debit;
    }

}

// Client code….assuming some delivery boy wants to get his payment

    // code from some method inside the delivery boy class... payment = 2.00; //

//  “I want my two dollars !”

    Wallet theWallet = myCustomer.getWallet();

if (theWallet.getTotalMoney() > payment)
{

    theWallet.subtractMoney(payment);
}
else
{

    // come back later and get my money
}