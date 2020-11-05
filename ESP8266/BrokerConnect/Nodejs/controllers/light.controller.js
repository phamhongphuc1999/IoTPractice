const { commonService } = require('../services')
const { json } = require("express")

const updateLightStatus = async(req, res) =>{
    let status = req.params.status;
    if(status == '1') {
        commonService.publish('esp8266', 1, false, {
            'status': 1
        })
    }
    else{
        commonService.publish('esp8266', 1, false, {
            'status': 0
        })
    }
    return res.status(200).json({
        'status': 1,
        message: 'success'
    })
}

module.exports = {
    updateLightStatus
}
