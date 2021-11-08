select companyname, round((sum(shippeddate > requireddate) + 0.0) / count(companyname) * 100, 2) as per
from 'order', shipper
where 'order'.employeeid = shipper.id
group by companyname
order by per desc;