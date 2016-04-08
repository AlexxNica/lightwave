﻿/*
 * Copyright © 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the “License”); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *·
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
using VMwareMMCIDP.UI.Common.Utilities;
[assembly: PermissionSetAttribute(SecurityAction.RequestMinimum, Unrestricted = true)]
    {

        private void InitializeComponent()
        {

        }
    }
    [SnapInSettings("{387738AF-C695-46f3-B178-9C9915364BD6}", DisplayName = "VMDirectory Browser",
            InitConsole();

        protected override void OnInitialize()
        {
            base.OnInitialize();

            VMDirEnvironment.Instance.SnapIn = this;
            MMCDlgHelper.snapIn = this;
        }
        
        void AddViewDescription(ScopeNode node, MmcListViewDescription lvd)
        {
            node.ViewDescriptions.Add(lvd);
            node.ViewDescriptions.DefaultIndex = 0;
            this.RootNode.Children.Add(node);
        }
        void InitConsole()
        {
            var il = new ImageList();
            this.SmallImages.AddStrip(VMDirEnvironment.Instance.GetToolbarImage());
            this.RootNode = new VMDirRootNode();
        }
        protected override void OnShutdown(AsyncStatus status)
            // saves data to local xml file
            VMDirEnvironment.Instance.SaveLocalData();
        }
    }