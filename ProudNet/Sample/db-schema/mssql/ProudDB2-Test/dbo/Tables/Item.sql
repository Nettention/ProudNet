﻿CREATE TABLE [dbo].[Item] (
    [UUID]      UNIQUEIDENTIFIER NOT NULL,
    [OwnerUUID] UNIQUEIDENTIFIER NOT NULL,
    [RootUUID]  UNIQUEIDENTIFIER NOT NULL,
    [Name] VARCHAR(50) NOT NULL,
	[Type]      INT              NOT NULL, 
    CONSTRAINT [PK_Item] PRIMARY KEY ([UUID])    
);

