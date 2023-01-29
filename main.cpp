



/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include <mutex>
#include "Std_Types.h"
#include "Can_GeneralTypes.h"
#include "Can_XLdriver_Internal.hpp"
#include "CanIf_Can.h"

using namespace std::chrono_literals;


void eventConsumer(const std::stop_token& stoken)
{
    unsigned int rcvSize = 1;
    XLevent xlEvent;
    while(!stoken.stop_requested())
    {
        rcvSize = 1;
        auto xlStatus = xlReceive(g_xlPortHandle, &rcvSize, &xlEvent);
        if (xlStatus != XL_ERR_QUEUE_IS_EMPTY && rcvSize > 0)
        {
            switch (xlEvent.tag)
            {
                case XL_RECEIVE_MSG:
                    if (xlEvent.tagData.msg.flags & XL_CAN_MSG_FLAG_TX_COMPLETED)
                    {
                        CanIf_TxConfirmation(static_cast<PduIdType>(xlEvent.tagData.msg.id));
                    }
                    else if(xlEvent.tagData.msg.flags == 0)
                    {
                        Can_HwHandleType hwHandle;
                        if(getHwHandle(Id2HwHandleMap, xlEvent.tagData.msg.id, hwHandle) == E_NOT_OK)
                        {
                            Can_XLdriver_DetReportError(CAN_XLDRIVER_MAIN_FUNCTION_READ_SID, CAN_E_PARAM_HANDLE);
                        }
                        else
                        {
                            Can_HwType mailbox{
                                    xlEvent.tagData.msg.id,
                                    hwHandle,
                                    xlEvent.chanIndex
                            };
                            PduInfoType pduInfo{
                                    xlEvent.tagData.msg.data,
                                    nullptr,
                                    CanData::getPayloadSize(static_cast<uint8>(xlEvent.tagData.msg.dlc))
                            };
                            CanIf_RxIndication(&mailbox, &pduInfo);
                        }
                    }
                    break;
                case XL_CHIP_STATE:
                    g_ChipState = xlEvent.tagData.chipState;
                    chipStateCV.notify_all();
                    break;
                default:
                    break;
            }
        }
    }
}


extern "C" Std_ReturnType Can_XLdriver_GetControllerErrorState(uint8 ControllerId, Can_ErrorStateType* ErrorStatePtr)
{
    Std_ReturnType ret = E_NOT_OK;
    std::mutex mtx;
    std::unique_lock lk(mtx);
    if(auto xLstatus = xlCanRequestChipState(g_xlPortHandle, 1 << ControllerId); (xLstatus == XL_SUCCESS) && (chipStateCV.wait_for(lk, 500ms) == std::cv_status::no_timeout))
    {
        switch (g_canFdSupport ? g_CanFdChipState.busStatus : g_ChipState.busStatus)
        {
            case XL_CHIPSTAT_BUSOFF:
                *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF;
                break;
            case XL_CHIPSTAT_ERROR_PASSIVE:
                *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE;
                break;
            case XL_CHIPSTAT_ERROR_WARNING:
            case XL_CHIPSTAT_ERROR_ACTIVE:
                *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE;
                break;
            default:
                break;
        }
        ret = E_OK;
    }
    else
    {
        Can_XLdriver_DetReportError(CAN_XLDRIVER_GET_CONTROLLER_ERROR_STATE_SID, CAN_E_PARAM_CONTROLLER);
    }
    return ret;
}



/**@} */
