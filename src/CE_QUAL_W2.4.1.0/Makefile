
# Compiler/Linker settings
FCOMP = gfortran
FCFLAGS = -O2 -shared -fPIC -Wall -g -Wextra -pedantic -Wline-truncation  -Wcharacter-truncation  -Wsurprising  -Waliasing  -Wimplicit-interface  -Wunused-parameter  -fwhole-file   -fbacktrace 
FCFLAGSEXE = -O2 -fPIC -Wall -g -Wextra -pedantic -Wline-truncation  -Wcharacter-truncation  -Wsurprising  -Waliasing  -Wimplicit-interface  -Wunused-parameter  -fwhole-file   -fbacktrace
LDFLAGS = -lgfortran
SHARED_LIB = CE_QUAL_W2.4.1.0.dylib
EXECUTABLE = CE_QUAL_W2.4.1.0.exe
SOURCEDIR = .

          
SOURCES = w2modules.f90 \
          aerate.f90 \
          az.f90 \
          balances.f90 \
          CEMA_Bubbles_Code_01.f90 \
          CEMA_FFT_Layer_01.f90 \
          CEMA_Input_01.f90 \
          CEMA_Input_Files_Read_01.f90 \
          CEMA_Output_01.f90 \
          CEMA_Sediment_Flux_Model_04.f90 \
          CEMA_Sediment_Model_03.f90 \
          CEMA_Turbidity_01.f90 \
          date.f90 \
          density.f90 \
          endsimulation.f90 \
          envir_perf.f90 \
          fishhabitat.f90 \
          gas-transfer.f90 \
          gate-spill-pipe.f90 \
          heat-exchange.f90 \
          hydroinout.f90 \
          init-cond.f90 \
          init-geom.f90 \
          init-u-elws.f90 \
          init.f90 \
          input.f90 \
          layeraddsub.f90 \
          macrophyte-aux.f90 \
          output.f90 \
          outputa2w2tools.f90 \
          outputinitw2tools.f90 \
          particle.f90 \
          restart.f90 \
          shading.f90 \
          tdg.f90 \
          temperature.f90 \
          time-varying-data.f90 \
          transport.f90 \
          update.f90 \
          screen_output.f90 \
          w2_4.f90 \
          water-quality.f90 \
          waterbody.f90 \
          withdrawal.f90 \
          wqconstituents.f90

OBJECTS = $(patsubst %.f90,%.o,$(SOURCES))

all: $(SHARED_LIB) $(EXECUTABLE)

$(SHARED_LIB): $(OBJECTS)
	$(FCOMP) $(FCFLAGS) $(LDFLAGS) $(OBJECTS) -o $(SHARED_LIB)  -I/usr/local/include -L/usr/local/lib
     
$(EXECUTABLE): $(OBJECTS)
	$(FCOMP) $(FCFLAGSEXE) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)  -I/usr/local/include -L/usr/local/lib

%.o: %.f90
	$(FCOMP) $(FCFLAGS) -c $< -o $@


clean:
	rm -rf *.o *.mod *.MOD *.dll *.dSYM *.dylib 
