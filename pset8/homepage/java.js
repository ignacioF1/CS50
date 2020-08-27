function thank() // Check if the form is compleate and thank or ask to fill it
{
    let fname = document.querySelector('#fname').value;
    let lname = document.querySelector('#lname').value;
    let email = document.querySelector('#email').value;

    if (fname === '' || lname === ''|| email === '' )
	{
        alert('Please fill in the form!');
        return true;
	}
	else
	{
	let name = document.querySelector('#fname').value;
	document.querySelector('#alert').innerHTML = ("Thank you "+ name + ", we'll be contacting you soon!");
	document.querySelector('#alert').hidden = false;

    navigator.geolocation.getCurrentPosition(function(position){
    let lat1 = position.coords.latitude;
    let lon1 = position.coords.longitude;

    const lat2 = -34.429913; // WoodLand coordinates -34.429913, -58.559551
    const lon2 = -58.559551;

    const R = 6371e3; // metres
    const φ1 = lat1 * Math.PI/180; // φ, λ in radians
    const φ2 = lat2 * Math.PI/180;
    const Δφ = (lat2-lat1) * Math.PI/180;
    const Δλ = (lon2-lon1) * Math.PI/180;

    const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
          Math.cos(φ1) * Math.cos(φ2) *
          Math.sin(Δλ/2) * Math.sin(Δλ/2);
    const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

    const d = Math.round(R * c / 1000); // in Km
    // Source: https://www.movable-type.co.uk/scripts/latlong.html
    //alert(d);
    dist = (d + "%");
    document.querySelector('#bar').style.width = dist; //Update the bar's width

    document.querySelector('#bar').innerHTML = 'You are at ' + d + 'Km'; //Update the bar's Km text
    return false;
    });
	}
}
