async function fetchEIDs() {
    const response = await fetch('/get_eids');
    const eids = await response.json();
    const eidTable = document.getElementById('eid-table');
    
    eids.forEach(eid => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${eid.register_number}</td>
            <td>${eid.EIS_information['Industry Identifier']}</td>
            <td>${eid.EIS_information['Specifying Telecommunication']}</td>
            <td>${eid.EIS_information['Country Code']}</td>
            <td>${eid.EIS_information['Issuer Identifier']}</td>
            <td>${eid.EIS_information['Platform Version']}</td>
            <td>${eid.EIS_information['OS Version']}</td>
            <td>${eid.EIS_information['Individual Identification Number']}</td>
            <td>${eid.EIS_information['Check Digits']}</td>
            <td>${eid.EID}</td>
        `;
        eidTable.appendChild(row);
    });
}

window.onload = fetchEIDs;

