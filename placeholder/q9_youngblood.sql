select distinct regiondescription, firstname, lastname, birthdate from (
select region.id as rid, regiondescription, firstname, lastname, birthdate
from employee, employeeterritory, territory, region
where employee.id = employeeterritory.employeeid and employeeterritory.territoryid = territory.id and territory.regionid = region.id
) as t
where not exists(
select * from 
(select region.id as rid, regiondescription, firstname, lastname, birthdate
from employee, employeeterritory, territory, region
where employee.id = employeeterritory.employeeid and employeeterritory.territoryid = territory.id and territory.regionid = region.id)
where rid = t.rid and birthdate < t.birthdate
)
order by rid;