const express = require('express')
const lightRoute = require('./light.route')

const router = express.Router()

router.use('/light', lightRoute)

module.exports = router;
