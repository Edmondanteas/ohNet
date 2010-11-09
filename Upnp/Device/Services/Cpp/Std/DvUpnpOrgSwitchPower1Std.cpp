#include <Std/DvUpnpOrgSwitchPower1.h>
#include <ZappTypes.h>
#include <DviService.h>
#include <Service.h>
#include <FunctorDvInvocation.h>

using namespace Zapp;

bool DvProviderUpnpOrgSwitchPower1Cpp::SetPropertyStatus(bool aValue)
{
    return SetPropertyBool(*iPropertyStatus, aValue);
}

void DvProviderUpnpOrgSwitchPower1Cpp::GetPropertyStatus(bool& aValue)
{
    aValue = iPropertyStatus->Value();
}

DvProviderUpnpOrgSwitchPower1Cpp::DvProviderUpnpOrgSwitchPower1Cpp(DvDeviceStd& aDevice)
    : DvProvider(aDevice.Device(), "upnp.org", "SwitchPower", 1)
{
    Functor empty;
    iPropertyStatus = new PropertyBool(new ParameterBool("Status"), empty);
    iService->AddProperty(iPropertyStatus); // passes ownership
}

void DvProviderUpnpOrgSwitchPower1Cpp::EnableActionSetTarget()
{
    Zapp::Action* action = new Zapp::Action("SetTarget");
    action->AddInputParameter(new ParameterBool("newTargetValue"));
    FunctorDvInvocation functor = MakeFunctorDvInvocation(*this, &DvProviderUpnpOrgSwitchPower1Cpp::DoSetTarget);
    iService->AddAction(action, functor);
}

void DvProviderUpnpOrgSwitchPower1Cpp::EnableActionGetTarget()
{
    Zapp::Action* action = new Zapp::Action("GetTarget");
    action->AddOutputParameter(new ParameterBool("RetTargetValue"));
    FunctorDvInvocation functor = MakeFunctorDvInvocation(*this, &DvProviderUpnpOrgSwitchPower1Cpp::DoGetTarget);
    iService->AddAction(action, functor);
}

void DvProviderUpnpOrgSwitchPower1Cpp::EnableActionGetStatus()
{
    Zapp::Action* action = new Zapp::Action("GetStatus");
    action->AddOutputParameter(new ParameterRelated("ResultStatus", *iPropertyStatus));
    FunctorDvInvocation functor = MakeFunctorDvInvocation(*this, &DvProviderUpnpOrgSwitchPower1Cpp::DoGetStatus);
    iService->AddAction(action, functor);
}

void DvProviderUpnpOrgSwitchPower1Cpp::DoSetTarget(IDvInvocation& aInvocation, TUint aVersion)
{
    aInvocation.InvocationReadStart();
    bool newTargetValue = aInvocation.InvocationReadBool("newTargetValue");
    aInvocation.InvocationReadEnd();
    SetTarget(aVersion, newTargetValue);
	aInvocation.InvocationWriteStart();
	aInvocation.InvocationWriteEnd();
}

void DvProviderUpnpOrgSwitchPower1Cpp::DoGetTarget(IDvInvocation& aInvocation, TUint aVersion)
{
    aInvocation.InvocationReadStart();
    aInvocation.InvocationReadEnd();
    bool respRetTargetValue;
    GetTarget(aVersion, respRetTargetValue);
	aInvocation.InvocationWriteStart();
    InvocationResponseBool respWriterRetTargetValue(aInvocation, "RetTargetValue");
    respWriterRetTargetValue.Write(respRetTargetValue);
	aInvocation.InvocationWriteEnd();
}

void DvProviderUpnpOrgSwitchPower1Cpp::DoGetStatus(IDvInvocation& aInvocation, TUint aVersion)
{
    aInvocation.InvocationReadStart();
    aInvocation.InvocationReadEnd();
    bool respResultStatus;
    GetStatus(aVersion, respResultStatus);
	aInvocation.InvocationWriteStart();
    InvocationResponseBool respWriterResultStatus(aInvocation, "ResultStatus");
    respWriterResultStatus.Write(respResultStatus);
	aInvocation.InvocationWriteEnd();
}

void DvProviderUpnpOrgSwitchPower1Cpp::SetTarget(uint32_t /*aVersion*/, bool /*anewTargetValue*/)
{
    ASSERTS();
}

void DvProviderUpnpOrgSwitchPower1Cpp::GetTarget(uint32_t /*aVersion*/, bool& /*aRetTargetValue*/)
{
    ASSERTS();
}

void DvProviderUpnpOrgSwitchPower1Cpp::GetStatus(uint32_t /*aVersion*/, bool& /*aResultStatus*/)
{
    ASSERTS();
}

