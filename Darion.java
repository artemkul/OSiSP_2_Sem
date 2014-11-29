package l2n.game.scripts.ai.raidboss;

import l2n.game.L2GameThreadPools;
import l2n.game.ai.Fighter;
import l2n.game.instancemanager.RaidBossSpawnManager;
import l2n.game.instancemanager.RaidBossSpawnManager.RaidBossStatus;
import l2n.game.model.L2Spawn;
import l2n.game.model.actor.L2Character;
import l2n.game.tables.DoorTable;
import l2n.game.tables.NpcTable;
import l2n.game.templates.actor.L2NpcTemplate;
import l2n.game.util.Location;

/**
 * управляет секретным проходам в Tower of Naia.
 * 
 * @author bloodshed (L2NextGen)
 * @date 26.07.2011
 * @time 14:28:58
 */
public class Darion extends Fighter
{
	private final class ActivateDarion implements Runnable
	{
		@Override
		public void run()
		{
			// если РБ Darion уже был убит
			if(RaidBossSpawnManager.getInstance().getRaidBossStatusId(NPC_ID) == RaidBossStatus.DEAD)
				dieDarion();
			else
				// TODO отправлять всех на 8 этаж
				spawnDarion();
		}
	}

	private static final int NPC_ID = 25603;
	private static final int TOLLES_CRYSTAL_PILLAR = 18506;

	// двери
	private final static int second_door1 = 20250005;
	private final static int second_door2 = 20250004;
	private final static int third_door1 = 20250006;
	private final static int third_door2 = 20250007;
	private final static int bridge_a = 20250778;
	private final static int bridge_b = 20250777;
	private final static int secret_a = 20250009;
	private final static int secret_b = 20250008;

	private L2Spawn crystal_pillar;

	public Darion(L2Character actor)
	{
		super(actor);
	}

	@Override
	protected void onEvtSpawn()
	{
		// при спауне запускаем таймер
		L2GameThreadPools.getInstance().scheduleAi(new ActivateDarion(), 15 * 1000, false);
		super.onEvtSpawn();
	}

	@Override
	protected void onEvtDead(L2Character killer)
	{
		dieDarion();
		super.onEvtDead(killer);
	}

	private void dieDarion()
	{
		DoorTable.getInstance().getDoor(second_door1).openMe();
		DoorTable.getInstance().getDoor(second_door2).openMe();
		DoorTable.getInstance().getDoor(third_door1).openMe();
		DoorTable.getInstance().getDoor(third_door2).openMe();

		DoorTable.getInstance().getDoor(bridge_a).closeMe();
		DoorTable.getInstance().getDoor(bridge_a).setGeoOpen(true);
		DoorTable.getInstance().getDoor(bridge_b).closeMe();
		DoorTable.getInstance().getDoor(bridge_b).setGeoOpen(true);

		DoorTable.getInstance().getDoor(secret_a).openMe();
		DoorTable.getInstance().getDoor(secret_b).openMe();

		try
		{
			L2NpcTemplate template = NpcTable.getTemplate(TOLLES_CRYSTAL_PILLAR);
			if(template == null)
			{
				_log.warn("Darion: mob with ID = " + TOLLES_CRYSTAL_PILLAR + " can't be spawned!");
				return;
			}
			crystal_pillar = new L2Spawn(template);
			crystal_pillar.setAmount(1);
			crystal_pillar.setLoc(new Location(21008, 244000, 11087));
			crystal_pillar.setRespawnDelay(0);
			crystal_pillar.doSpawn(true);
		}
		catch(Exception e)
		{
			_log.warn("Tully: error spawn self_destructer: ", e);
		}
	}

	private void spawnDarion()
	{
		DoorTable.getInstance().getDoor(secret_a).closeMe();
		DoorTable.getInstance().getDoor(secret_b).closeMe();

		DoorTable.getInstance().getDoor(bridge_a).openMe();
		DoorTable.getInstance().getDoor(bridge_a).setGeoOpen(false);
		DoorTable.getInstance().getDoor(bridge_b).openMe();
		DoorTable.getInstance().getDoor(bridge_b).setGeoOpen(false);

		DoorTable.getInstance().getDoor(second_door1).closeMe();
		DoorTable.getInstance().getDoor(second_door2).closeMe();
		DoorTable.getInstance().getDoor(third_door1).closeMe();
		DoorTable.getInstance().getDoor(third_door2).closeMe();

		if(crystal_pillar != null)
		{
			crystal_pillar.despawnAll();
			crystal_pillar = null;
		}
	}
}
