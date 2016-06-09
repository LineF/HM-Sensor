package main;

use strict;
use warnings;

# device definition
# $HMConfig::culHmModel{'F101'} = {name => 'HB-UW-Sen-THPL-I', st => 'THPLSensor', cyc => '00:10', rxt => 'l:w:c:f', lst  => 'p',   chn  => '',};
$HMConfig::culHmModel{'F201'} = {name => 'HB-UW-Sen-TH-Pn', st => 'THPnSensor', cyc => '00:10', rxt => 'w:c', lst  => 'p',   chn  => '',};

$HMConfig::culHmRegDefine{'lowBatLimitTH'} = {a=>18.0,s=>1.0,l=>0,min=>1.0,max=>5,c=>'',f=>10,u=>'V',d=>0,t=>'Low batterie limit, step 0.1 V.'};
$HMConfig::culHmRegDefine{'oscCal'} = {a=>35.0,s=>1.0,l=>0,min=>0,max=>255,c=>'',f=>'',u=>'',d=>0,t=>'OSCCAL: calibration value of controller frequency'};

# Register model mapping
$HMConfig::culHmRegModel{'HB-UW-Sen-TH-Pn'} = {
	'burstRx'         => 1,
	'lowBatLimitTH'   => 1,
	'ledMode'         => 1,
	'transmDevTryMax' => 1,
	'oscCal'          => 1
#	,'altitude'        => 1
};

# subtype channel mapping
$HMConfig::culHmSubTypeSets{'THPnSensor'}    = {
	'peerChan'       => '0 <actChn> ... single [set|unset] [actor|remote|both]',
	'fwUpdate'       => '<filename> <bootTime> ...',
	'getSerial'      => '',
	'getVersion'     => '',
	'statusRequest'  => '',
	'burstXmit'      => ''
};

# Subtype spezific funtions
sub CUL_HM_ParseTHPnSensor(@){
	
	my ($mFlg, $frameType, $src, $dst, $msgData, $targetDevIO) = @_;
	
	my $shash = CUL_HM_id2Hash($src);                                           #sourcehash - will be modified to channel entity
	my @events = ();

	# WEATHER_EVENT
	if ($frameType eq '70'){
		my $name = $shash->{NAME};
		my $chn = '01';

		my ($dTempBat, $humidity, $batVoltage) = map{hex($_)} unpack ('A4A2A4', $msgData);

		# temperature
		my $temperature =  $dTempBat & 0x7fff;
		$temperature = ($temperature &0x4000) ? $temperature - 0x8000 : $temperature; 
		$temperature = sprintf('%0.1f', $temperature / 10);

		my $stateMsg = 'state:T: ' . $temperature;
		push (@events, [$shash, 1, 'temperature:' . $temperature]);

		# battery state
		push (@events, [$shash, 1, 'battery:' . ($dTempBat & 0x8000 ? 'low' : 'ok')]);

		# battery voltage
		$batVoltage = sprintf('%.2f', (($batVoltage + 0.00) / 100));
		push (@events, [$shash, 1, 'batVoltage:' . $batVoltage]);

		# humidity
		if ($humidity)                 {
			$stateMsg .= ' H: ' . $humidity;
			push (@events, [$shash, 1, 'humidity:' . $humidity]);
		}
		
		push (@events, [$shash, 1, $stateMsg]);
	}

	return @events;
}


1;
