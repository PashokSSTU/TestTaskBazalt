# branch_cli
### Description
This utility is designed to compare package data contained in two selected ALT Linux repositories.

*Calling example:*
```
branch_cli -b p9 p10 -o /root/compare/p9_p10.json
```

The result of the utility execution is written to a .json file, the path to which is specified in the call.

#### Result file format
```
{
	"branches" : [
	    /* packages contained in this repository but missing from another repository
	    but missing from another repository */
		"missing from <second branch name>": [
		{ // Package data
			"name": "",
			"epoch": ,
			"version": "",
			"release": "",
			"arch": "",
			"disttag": "",
			"buildtime": ,
			"source": ""
		},
		.
		.
		.
		]
		
	    /* packages contained in this repository but missing from another repository
	    but missing from another repository */
		"missing from <first branch name>": [
		{
			// Package data
		},
		.
		.
		.
		]
		
		/* packages from the first repository whose release version is greater than the version of the same package from another repository */
		"releases" : [
		{
			// Package data
		},
		.
		.
		.
		]
	]
}
```

### Building
1. Invoke this command to install the required packages:
```
apt-get -y install git cmake make ninja-build jsoncpp-devel curl-devel cli11-devel boost-devel
```

2. Clone the repository and go to it. After that, call the build script:
```
cd <path to directiry>/TestTaskBazalt/
./build.sh
```
3. Wait for project build;
4. The installation of the built library and utility is done automatically after the build is invoked. The files are installed in directories according to the FHS standard.


