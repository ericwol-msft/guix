/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Vertical List Management                                            */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vertical_list_scroll_info_get                   PORTABLE C      */
/*                                                           6.4.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets the scrollbar information.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    win                                   Pointer to window             */
/*    style                                 Style                         */
/*    info                                  Information of scrollbar      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_first_client_child_get            Get the first client child    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2023     Ting Zhu                 Modified comments(s),         */
/*                                            improved the calculation of */
/*                                            the maximum scrolling value.*/
/*                                            resulting in version 6.4.0  */
/*                                                                        */
/**************************************************************************/
VOID _gx_vertical_list_scroll_info_get(GX_VERTICAL_LIST *list, ULONG style, GX_SCROLL_INFO *info)
{
INT        value;
GX_VALUE   height;
GX_WIDGET *child;

    GX_PARAMETER_NOT_USED(style);

    if (list -> gx_vertical_list_callback)
    {
        /* If list callback is set, children winthin the list should share the same height. */
        info -> gx_scroll_maximum = (list->gx_vertical_list_total_rows * list -> gx_vertical_list_child_height - 1);
    }
    else
    {
        info -> gx_scroll_maximum = 0;

        child = _gx_widget_first_client_child_get((GX_WIDGET *)list);

        while (child)
        {
            _gx_widget_height_get(child, &height);

            info -> gx_scroll_maximum += height;

            child = _gx_widget_next_client_child_get(child);
        }
    }
    info -> gx_scroll_minimum = 0;
    info -> gx_scroll_visible = (GX_VALUE)(list -> gx_window_client.gx_rectangle_bottom - list -> gx_window_client.gx_rectangle_top + 1);

    if (info -> gx_scroll_maximum <= info -> gx_scroll_visible)
    {
        info -> gx_scroll_value = 0;
        info -> gx_scroll_increment = 0;
        info -> gx_scroll_maximum = info->gx_scroll_visible;
        return;
    }

    value = list -> gx_vertical_list_top_index * list -> gx_vertical_list_child_height;

    if (list -> gx_vertical_list_top_index >= 0)
    {
        child = _gx_widget_first_client_child_get((GX_WIDGET *)list);

        if (child)
        {
            value += list -> gx_window_client.gx_rectangle_top - child -> gx_widget_size.gx_rectangle_top;
        }
    }

    if (value < info -> gx_scroll_minimum)
    {
        value = info -> gx_scroll_minimum;
    }
    else
    {
        if (value > info -> gx_scroll_maximum - info -> gx_scroll_visible + 1)
        {
            value = info -> gx_scroll_maximum - info -> gx_scroll_visible + 1;
        }
    }

    info -> gx_scroll_value = value;
    info -> gx_scroll_increment = list -> gx_vertical_list_child_height / 2;
}

