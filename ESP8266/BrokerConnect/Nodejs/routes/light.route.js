const express = require('express')
const { lightController } = require('../controllers')

const router = express.Router();

router.route('/:status')
    .post(lightController.updateLightStatus)

module.exports = router
