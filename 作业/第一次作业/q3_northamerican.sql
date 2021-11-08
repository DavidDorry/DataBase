select id, shipcountry, case shipcountry when 'USA' then 'NorthAmerica' when 'Mexico' then 'NorthAmerica' when 'Canada' then 'NorthAmerica' else 'OtherPlace' end
from 'order'
where id >= 15445
order by id
limit 0, 20;