select *
from (select categoryname, count(categoryname) as ct, round(avg(unitprice), 2), min(unitprice), max(unitprice), sum(unitsonorder)
from product, category
where product.categoryid = category.id
group by categoryname
order by category.id)
where ct > 10;