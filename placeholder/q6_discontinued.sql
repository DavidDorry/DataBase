select productname, companyname, contactname
from (select productname, companyname, contactname, orderdate
from product, orderdetail, 'order', customer
where product.discontinued = 1 and product.id = orderdetail.productid and orderdetail.orderid = 'order'.id and 'order'.customerid = customer.id) as t
where not exists(select * from (select productname, companyname, contactname, orderdate
from product, orderdetail, 'order', customer
where product.discontinued = 1 and product.id = orderdetail.productid and orderdetail.orderid = 'order'.id and 'order'.customerid = customer.id) 
where productname = t.productname and orderdate < t.orderdate)
order by productname;