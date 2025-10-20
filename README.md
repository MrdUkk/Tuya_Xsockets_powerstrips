# About
This a replacement custom made, firmware for ZigBee Tuya-ecosystem based power socket strips that's hardware is based on
Tuya [ZT3L](https://developer.tuya.com/en/docs/iot/zt3l-module-datasheet?id=Ka438n1j8nuvu) or [ZTU](https://developer.tuya.com/en/docs/iot/ztu-module-datasheet?id=Ka45nl4ywgabp) models chips only (in home automations software its IEEE MAC address displayed starts with a4:c1:38). 
These modules contains TLSR8258 with 1M FLASH (Telink Semi TLSR8x-series microcontrollers).


# Description
This is a PoC firmware written from Telink's sampleSwitch ZigBee SDK example. I was starting from the
ground learning of ZigBee Telink Stack and TLSR8 microcontroller while digging available source code internals.
This process was very interesting and took me 3 days of work. The documentation to SDK is rather poor as i see it and there are practically only 
few source code examples without extensive description but with a bugs.
I provide only source files. no makefiles. Builded using Telink IoT Studio IDE with SDK V3.7.1.4.
Need to learn additionally how to properly setup new project in Telinks own way without unnessary references.


My firmware exposes standard number of ZigBee ZCL clusters: Basic/Identify/OnOff/Scenes/Groups.
Physical button on strip momentary push toggles on/off between all 5 sockets. and holding it pressed for five
seconds initialize ZigBee network pairing.
On Basic cluster command 0x0000 (reset_fact_default) supported resetting saved in flash configuration and restarting microcontroller.
Supports standard zigbee direct binding feature (appears in this list on HA)
Because i needed to support OTA firmware updates i need to set unique MANUFACTURER ID 
or update process can potentially interfere with already existed device on a market.
I can not allocate it via standard ZigBee Alliance way so decided to set it some random upper 16bit value.
Observed interested behavior in current consumption: original firmware draws 21mA in idle condition and my firmware only 12mA.
When all five sockets turned ON - current draw peaked at 340mA @ 5V.


# HOWTO
1. place code under Telink SDK tl_zigbee_sdk\apps\5socketsPowerSwitch directory
2. edit tl_zigbee_sdk\build\tlsr_tc32\.cproject file adding new section
```
    <cconfiguration id="com.telink.tc32eclipse.configuration.app.debug.1775306817.1609575092.51908207.1450330976.401297445.921750203">
			<storageModule buildSystemId="org.eclipse.cdt.managedbuilder.core.configurationDataProvider" id="com.telink.tc32eclipse.configuration.app.debug.1775306817.1609575092.51908207.1450330976.401297445.921750203" moduleId="org.eclipse.cdt.core.settings" name="5socketsPowerSwitch">
				<externalSettings/>
				<extensions>
					<extension id="org.eclipse.cdt.core.ELF" point="org.eclipse.cdt.core.BinaryParser"/>
					<extension id="org.eclipse.cdt.core.GASErrorParser" point="org.eclipse.cdt.core.ErrorParser"/>
					<extension id="org.eclipse.cdt.core.GLDErrorParser" point="org.eclipse.cdt.core.ErrorParser"/>
					<extension id="org.eclipse.cdt.core.MakeErrorParser" point="org.eclipse.cdt.core.ErrorParser"/>
					<extension id="org.eclipse.cdt.core.GCCErrorParser" point="org.eclipse.cdt.core.ErrorParser"/>
				</extensions>
			</storageModule>
			<storageModule moduleId="cdtBuildSystem" version="4.0.0">
				<configuration artifactName="${ConfigName}" buildArtefactType="com.telink.tc32eclipse.buildArtefactType.app" buildProperties="org.eclipse.cdt.build.core.buildArtefactType=com.telink.tc32eclipse.buildArtefactType.app,org.eclipse.cdt.build.core.buildType=org.eclipse.cdt.build.core.buildType.debug" description="5 sockets unTuyed power strip firmware" id="com.telink.tc32eclipse.configuration.app.debug.1775306817.1609575092.51908207.1450330976.401297445.921750203" name="5socketsPowerSwitch" optionalBuildProperties="org.eclipse.cdt.docker.launcher.containerbuild.property.selectedvolumes=,org.eclipse.cdt.docker.launcher.containerbuild.property.volumes=" parent="com.telink.tc32eclipse.configuration.app.debug" postbuildStep="&quot;../../../tools/tl_check_fw.sh&quot; ${ConfigName} tc32" prebuildStep="&quot;../../../tools/tl_link_load.sh&quot; &quot;../../../platform/boot/8258/boot_8258.link&quot; &quot;${workspace_loc:/${ProjName}}/boot.link&quot;">
					<folderInfo id="com.telink.tc32eclipse.configuration.app.debug.1775306817.1609575092.51908207.1450330976.401297445.921750203." name="/" resourcePath="">
						<toolChain id="com.telink.tc32eclipse.toolchain.TC32Win.app.debug.1509434107" name="TC32-GCC Toolchain" superClass="com.telink.tc32eclipse.toolchain.TC32Win.app.debug">
							<option id="com.telink.tc32eclipse.toolchain.options.toolchain.objcopy.flash.app.debug.1296246444" name="Generate binary file for Flash memory" superClass="com.telink.tc32eclipse.toolchain.options.toolchain.objcopy.flash.app.debug"/>
							<option id="com.telink.tc32eclipse.toolchain.options.toolchain.objdump.app.debug.1983834865" name="Generate Extended Listing (Source + generated Assembler)" superClass="com.telink.tc32eclipse.toolchain.options.toolchain.objdump.app.debug"/>
							<option id="com.telink.tc32eclipse.toolchain.options.toolchain.size.app.debug.720953975" name="Print Size" superClass="com.telink.tc32eclipse.toolchain.options.toolchain.size.app.debug"/>
							<targetPlatform id="com.telink.tc32eclipse.targetplatform.TC32Win.app.debug.1415322813" name="TC32 Cross-Target" superClass="com.telink.tc32eclipse.targetplatform.TC32Win.app.debug"/>
							<builder arguments="-j4" buildPath="${workspace_loc:/tl_zigbee_sdk/Debug}" command="make" id="com.telink.tc32eclipse.target.builder.TC32Win.app.debug.1711699552" keepEnvironmentInBuildfile="false" managedBuildOn="true" name="GNU Make Builder" superClass="com.telink.tc32eclipse.target.builder.TC32Win.app.debug"/>
							<tool id="com.telink.tc32eclipse.tool.assembler.TC32Win.app.debug.335664613" name="TC32 CC/Assembler" superClass="com.telink.tc32eclipse.tool.assembler.TC32Win.app.debug">
								<option id="com.telink.tc32eclipse.assembler.option.debug.level.499402625" name="Generate Debugging Info" superClass="com.telink.tc32eclipse.assembler.option.debug.level" value="com.telink.tc32eclipse.assembler.option.debug.level.none" valueType="enumerated"/>
								<option id="com.telink.tc32eclipse.asm.option.flags.1162724729" name="Other GCC Flags" superClass="com.telink.tc32eclipse.asm.option.flags" value="-DMCU_CORE_8258 -DMCU_STARTUP_8258" valueType="string"/>
								<option IS_BUILTIN_EMPTY="false" IS_VALUE_EMPTY="false" id="com.telink.tc32eclipse.asm.option.include.paths.714338878" name="#include paths for preprocessor(-I)" superClass="com.telink.tc32eclipse.asm.option.include.paths" useByScannerDiscovery="false" valueType="includePath">
									<listOptionValue builtIn="false" value="../../../apps/5socketsPowerSwitch"/>
								</option>
								<inputType id="com.telink.tc32eclipse.tool.assembler.input.453051885" superClass="com.telink.tc32eclipse.tool.assembler.input"/>
							</tool>
							<tool command="tc32-elf-gcc -ffunction-sections -fdata-sections" id="com.telink.tc32eclipse.tool.compiler.TC32Win.app.debug.586040477" name="TC32 Compiler" superClass="com.telink.tc32eclipse.tool.compiler.TC32Win.app.debug">
								<option id="com.telink.tc32eclipse.compiler.option.debug.level.1078837975" name="Generate Debugging Info" superClass="com.telink.tc32eclipse.compiler.option.debug.level" value="com.telink.tc32eclipse.compiler.option.debug.level.none" valueType="enumerated"/>
								<option id="com.telink.tc32eclipse.compiler.option.optimize.1685100106" name="Optimization Level" superClass="com.telink.tc32eclipse.compiler.option.optimize" value="com.telink.tc32eclipse.compiler.optimize.two" valueType="enumerated"/>
								<option IS_BUILTIN_EMPTY="false" IS_VALUE_EMPTY="false" id="com.telink.tc32eclipse.compiler.option.def.509979469" name="Define Syms (-D)" superClass="com.telink.tc32eclipse.compiler.option.def" valueType="definedSymbols">
									<listOptionValue builtIn="false" value="END_DEVICE=1"/>
									<listOptionValue builtIn="false" value="MCU_CORE_8258=1"/>
									<listOptionValue builtIn="false" value="__PROJECT_TL_SWITCH__=1"/>
								</option>
								<option id="com.telink.tc32eclipse.compiler.option.optimize.packstruct.1140699522" name="Pack structs (-fpack-struct)" superClass="com.telink.tc32eclipse.compiler.option.optimize.packstruct" value="true" valueType="boolean"/>
								<option id="com.telink.tc32eclipse.compiler.option.optimize.shortenums.45601854" name="Short enums (-fshort-enums)" superClass="com.telink.tc32eclipse.compiler.option.optimize.shortenums" value="true" valueType="boolean"/>
								<option id="com.telink.tc32eclipse.compiler.option.optimize.other.168051236" name="Other Optimization Flags" superClass="com.telink.tc32eclipse.compiler.option.optimize.other" value="-finline-small-functions" valueType="string"/>
								<option id="com.telink.tc32eclipse.compiler.option.std.208931104" name="Language Standard" superClass="com.telink.tc32eclipse.compiler.option.std" value="com.telink.tc32eclipse.compiler.option.std.gnu99" valueType="enumerated"/>
								<option id="com.telink.tc32eclipse.compiler.option.otherflags.1972466733" name="Other flags" superClass="com.telink.tc32eclipse.compiler.option.otherflags" value="-fshort-wchar -fms-extensions" valueType="string"/>
								<option IS_BUILTIN_EMPTY="false" IS_VALUE_EMPTY="false" id="com.telink.tc32eclipse.compiler.option.incpath.675510467" name="Include Paths (-I)" superClass="com.telink.tc32eclipse.compiler.option.incpath" useByScannerDiscovery="false" valueType="includePath">
									<listOptionValue builtIn="false" value="../../../apps/5socketsPowerSwitch/common"/>
									<listOptionValue builtIn="false" value="../../../apps/5socketsPowerSwitch"/>
									<listOptionValue builtIn="false" value="../../../platform"/>
									<listOptionValue builtIn="false" value="../../../proj/common"/>
									<listOptionValue builtIn="false" value="../../../proj"/>
									<listOptionValue builtIn="false" value="../../../zigbee/common/includes"/>
									<listOptionValue builtIn="false" value="../../../zigbee/zbapi"/>
									<listOptionValue builtIn="false" value="../../../zigbee/bdb/includes"/>
									<listOptionValue builtIn="false" value="../../../zigbee/zcl"/>
									<listOptionValue builtIn="false" value="../../../zigbee/ota"/>
									<listOptionValue builtIn="false" value="../../../zbhci"/>
								</option>
								<inputType id="com.telink.tc32eclipse.compiler.TC32Win.input.2066013394" name="C Source Files" superClass="com.telink.tc32eclipse.compiler.TC32Win.input"/>
							</tool>
							<tool command="tc32-elf-ld --gc-sections" id="com.telink.tc32eclipse.tool.linker.TC32Win.app.debug.1309316579" name="TC32 C Linker" superClass="com.telink.tc32eclipse.tool.linker.TC32Win.app.debug">
								<option IS_BUILTIN_EMPTY="false" IS_VALUE_EMPTY="false" id="com.telink.tc32eclipse.linker.option.libs.749436671" name="Libraries (-l)" superClass="com.telink.tc32eclipse.linker.option.libs" valueType="libs">
									<listOptionValue builtIn="false" value="zb_ed"/>
									<listOptionValue builtIn="false" value="drivers_8258"/>
								</option>
								<option IS_BUILTIN_EMPTY="false" IS_VALUE_EMPTY="false" id="com.telink.tc32eclipse.linker.option.libpath.1347338607" name="Libraries Path (-L)" superClass="com.telink.tc32eclipse.linker.option.libpath" valueType="stringList">
									<listOptionValue builtIn="false" value="../../../zigbee/lib/tc32"/>
									<listOptionValue builtIn="false" value="../../../platform/lib"/>
								</option>
								<inputType id="com.telink.tc32eclipse.tool.linker.input.1707435231" name="OBJ Files" superClass="com.telink.tc32eclipse.tool.linker.input">
									<additionalInput kind="additionalinputdependency" paths="$(USER_OBJS)"/>
									<additionalInput kind="additionalinput" paths="$(LIBS)"/>
								</inputType>
							</tool>
							<tool id="com.telink.tc32eclipse.tool.archiver.TC32Win.base.1221125955" name="TC32 Archiver" superClass="com.telink.tc32eclipse.tool.archiver.TC32Win.base"/>
							<tool id="com.telink.tc32eclipse.tool.objdump.TC32Win.app.debug.1418934386" name="TC32 Create Extended Listing" superClass="com.telink.tc32eclipse.tool.objdump.TC32Win.app.debug">
								<option id="com.telink.tc32eclipse.objdump.option.output.1861852372" name="Output Filename" superClass="com.telink.tc32eclipse.objdump.option.output" value="${ConfigName}.lst" valueType="string"/>
							</tool>
							<tool id="com.telink.tc32eclipse.tool.objcopy.flash.TC32Win.app.debug.1965333113" name="TC32 Create Flash image" superClass="com.telink.tc32eclipse.tool.objcopy.flash.TC32Win.app.debug">
								<option id="com.telink.tc32eclipse.objcopy.option.flash.stdopts.873254799" name="Options to create flash image" superClass="com.telink.tc32eclipse.objcopy.option.flash.stdopts" value="" valueType="string"/>
								<option id="com.telink.tc32eclipse.objcopy.flash.option.output.1070485958" name="Output Filename" superClass="com.telink.tc32eclipse.objcopy.flash.option.output" value="" valueType="string"/>
							</tool>
							<tool id="com.telink.tc32eclipse.tool.size.TC32Win.app.debug.1193229003" name="Print Size" superClass="com.telink.tc32eclipse.tool.size.TC32Win.app.debug"/>
						</toolChain>
					</folderInfo>
					<sourceEntries>
						<entry excluding="apps/common/module_test.c|apps/common|apps/bootLoader|apps/sampleSwitch|apps/sampleContactSensor|platform/chip_826x|apps/sampleGW|apps/sampleLight|platform/chip_8278" flags="VALUE_WORKSPACE_PATH|RESOLVED" kind="sourcePath" name=""/>
					</sourceEntries>
				</configuration>
			</storageModule>
			<storageModule moduleId="org.eclipse.cdt.core.externalSettings"/>
			<storageModule moduleId="org.eclipse.cdt.internal.ui.text.commentOwnerProjectMappings"/>
			<storageModule moduleId="org.eclipse.cdt.core.language.mapping"/>
			<storageModule moduleId="ilg.gnuarmeclipse.managedbuild.packs"/>
		</cconfiguration>
```
3. save file and launch TElink studio IDE
4. compile
5. grab final file under tl_zigbee_sdk\build\tlsr_tc32\5socketsPowerSwitch\5socketsPowerSwitch.bin
6. use Telink's original BDT debugger and flasher to upload into uC that file. OR use famous [TLSRx flasher by pvvx](https://github.com/pvvx/TlsrComProg825x)

# Supported models:
ts011f tz3000_gdyjfvgm

Any other model can be added easy by modifying board_8258.h file assigning proper GPIOs 
AND modifying sampleSwitchEpCfg.c when number of sockets differs from 5 like in my device.

After firmware is flashed device will be ZED (ZigBee End Device) and seen as Manufacturer = Wholesome Software, and model = 5SockPowerStrip-1.
Leaving manufacturer and model id original is a no-way to go because Home Assistant/3rd home automation systems will anyway use Tuya ecosystem quirks when see original ID/model.



## TODO:
- Implement 2 steps OTA firmware so anyone from Home Assistant can do upgrade from Tuya flawlessly
- Investigate Groups/Scenes ZCL clusters. Possibly it was not needed for power sockets
- Implement zcl static array structure sizes automatic calculation at compile time
- Investigate why globally allocated structures are needed by SDK, may be some RAM can be saved 
- Code polishing and bug hunting


## Acknowledgements
- [@slacky1965](https://github.com/slacky1965)

