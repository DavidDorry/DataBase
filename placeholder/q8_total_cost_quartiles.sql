select companyname, id, expend
from (select ntile(4) over(order by expend) as gid, companyname, id, expend
from (select companyname, customer.id as id, round(sum(unitprice * quantity), 2) as expend
from orderdetail, 'order', customer
where orderdetail.orderid = 'order'.id and 'order'.customerid = customer.id
group by companyname, customer.id))
where gid = 1;