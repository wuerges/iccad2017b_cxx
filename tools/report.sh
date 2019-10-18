COMMIT=`git log -1 --format=format:%h`
echo "Running router for ${COMMIT}"
FILE=results/${COMMIT}.result
lscpu |grep model |tee -a ${FILE}
echo $@ |tee -a ${FILE}
./build/app/RoutingReport testcases/official/original/case* testcases/official/alpha/caseA testcases/official/hidden/case* | tee -a ${FILE}
