WITH RECURSIVE t(n, id) AS (
    values('', 1)
  UNION ALL
    SELECT t.n||', '||c.productname, id + 1 FROM t, (select productname, row_number() over() as num from (select productname
from customer, 'order', orderdetail, product
where customer.id = 'order'.customerid and 'order'.id = orderdetail.orderid and productid = product.id and companyname = 'Queen Cozinha' and orderdate like '%2014-12-25%'
order by productid)) c where t.id = c.num
)
SELECT max(n) FROM t;